#include "SceneObject.h"
#include "Collider.h"
#include "AABox.h"

using namespace lasmath;

//TODO commenting

SceneObject::SceneObject() : m_parent(nullptr), m_collider(nullptr), m_childrenLocked(false), m_deletionFlag(false), m_transferFlag(false), m_transferTargetFlag(false)
{
	m_localTransform.setIdentity();
}


SceneObject::~SceneObject()
{
	// Destroy all children
	m_childrenLocked = true;
	for (auto child : m_children) {
		delete child;
	}
	m_childrenLocked = false;
	// Destroy collider
	delete m_collider;
}

bool SceneObject::addChild(SceneObject * child)
{
	// Check if child already has a parent
	if (child->m_parent == nullptr) {
		// Check if modifying m_children is safe
		if (!m_childrenLocked) {
			// If safe, add child to m_children and set this as its parent
			child->m_parent = this;
			m_children.push_back(child);
		}
		else {
			// If not safe, queue child for addition later
			m_toAdd.push_back(child);
		}
		// Return true for success
		return true;
	}
	else {
		//If child has parent, adding child fails
		return false;
	}
}

bool SceneObject::removeChild(SceneObject * child)
{
	// Check if child is safe to delete, and is a child of this SceneObject
	if (child->safeToDelete() && child->m_parent==this) {
		// Check if modifying m_children is safe
		if (!m_childrenLocked) {
			// If safe, destroy child and erase pointer from m_children
			m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
			delete child;
		}
		else {
			// If not safe, queue child for deletion later
			m_toDelete.push_back(child);
			// flag child as to be deleted
			child->m_deletionFlag = true;
		}
		// Child has been, or will be, deleted
		return true;
	}
	else {
		// Child will not be deleted
		return false;
	}
}

bool SceneObject::transferChild(SceneObject * child, SceneObject * target)
{
	//Check if transfer is safe and valid
	if (child->safeForTransfer()			// Child not marked for deletion or transfer
		&& !target->markedForDeletion()		// Target not marked for deletion	
		&& child != target					// Child is not target
		&& child->m_parent == this) {		// This is parent of child
		// Queue pair of child and target to be transferred later
		std::pair<SceneObject*, SceneObject*> arguments = std::make_pair(child, target);
		m_toTransfer.push_back(arguments);
		// flag child as to be transferred
		child->m_transferFlag = true;
		// flag target as target of transfer
		target->m_transferTargetFlag = true;
		// return true if transfer is valid
		return true;
	}
	else {
		// Transfer will not happen
		return false;
	}
}

void SceneObject::setLocalTransform(const Matrix3 & transformation)
{
	m_localTransform = transformation;
}

Matrix3 SceneObject::getLocalTransform()
{
	return m_localTransform;
}

Matrix3 SceneObject::getGlobalTransform()
{
	return m_globalTransform;
}

void SceneObject::rotate(float angle)
{
	// Create matrix rotating X and Y axes
	Matrix3 rotation;
	rotation.setRotateZ(angle);
	// Apply rotation to local transform
	m_localTransform = m_localTransform * rotation;
}

void SceneObject::scale(const Vector2 & proportions)
{
	// Create matrix scaling X and Y axes
	Matrix3 scale = { proportions[0],0,0,
						0,proportions[1],0,
						0,0,1 };
	// Apply to local transform
	m_localTransform = m_localTransform * scale;
}

void SceneObject::translate(const Vector2 & vec)
{
	// Create matrix translating by vec
	Matrix3 translate;
	translate.setIdentity();
	translate[2] = { vec[0],vec[1],1 };
	// Apply translation to local transform
	m_localTransform = m_localTransform * translate;
}

void SceneObject::transform(const Matrix3 & transformation)
{
	m_localTransform = m_localTransform * transformation;
}

void SceneObject::globalTranslate(const Vector2 & vec)
{
	Vector2 translation = vec;
	// Check if this has a parent
	if (m_parent != nullptr) {
		// Transform translation by inverse of parent's global position
		Vector3 localTranslation = (Vector3)translation;
		m_parent->getGlobalTransform().transformByInverse(localTranslation);
		translation = (Vector2)localTranslation;
	}
	// Add translation to local transform's position
	m_localTransform[2] = m_localTransform[2] + (Vector3)translation;
}

void SceneObject::update(float deltaTime)
{
	// Calculate new global transform
	calculateGlobalTransform();
	// Place collider
	setupCollider();
	// Call update on all children
	m_childrenLocked = true;			// lock m_children
	for (auto child : m_children) {
		child->update(deltaTime);
	}
	m_childrenLocked = false;			// unlock m_children
}

void SceneObject::draw(aie::Renderer2D * renderer)
{
	// draw all children
	m_childrenLocked = true;			// lock m_children
	for (auto child : m_children) {
		child->draw(renderer);
	}
	m_childrenLocked = false;			// unlock m_children
}

void SceneObject::updateChildList()
{
	// Call updateChildList for all children
	// This is done first so if a child transfers a SceneObject to its parent
	// the transfer will occur in the same frame
	m_childrenLocked = true;			// lock m_children
	for (auto child : m_children) {
		child->updateChildList();
	}
	m_childrenLocked = false;			// unlock m_children
	// Transfer children in toTransfer list
	for (auto arguments : m_toTransfer) {
		performChildTransfer(arguments.first, arguments.second);
	}
	// clear toTransfer list
	m_toTransfer.clear();
	// Add SceneObjects in toAdd list
	for (auto child : m_toAdd) {
		addChild(child);
	}
	// clear toAdd list
	m_toAdd.clear();
	// Remove and delete all children in toDelete list
	for (auto child : m_toDelete) {
		removeChild(child);
	}
	// clear toDelete list
	m_toDelete.clear();
	// Reset all deletion and transfer flags to false
	m_deletionFlag = false;
	m_transferFlag = false;
	m_transferTargetFlag = false;
}

bool SceneObject::safeToDelete()
{
	// Safe to delete if not marked to be tranferred or as target of transfer
	bool safeToDelete = !(m_transferFlag || m_transferTargetFlag);
	if (safeToDelete) {
		// If any child not safe to delete, this is not safe to delete
		for (auto child : m_children) {
			if (!child->safeToDelete()) {
				safeToDelete = false;
				break;
			}
		}
	}
	return safeToDelete;
}

bool SceneObject::markedForDeletion()
{
	// Marked for deletion if this or any parent has m_deletionFlag true
	bool marked = m_deletionFlag;
	if (!marked) {
		if (m_parent != nullptr && m_parent->markedForDeletion()) {
			marked = true;
		}
	}
	return marked;
}

bool SceneObject::safeForTransfer()
{
	// Safe to transfer if not marked for deletion or to be transferred
	return !(markedForDeletion()||m_transferFlag);

}

SceneObject * SceneObject::getParent()
{
	return m_parent;
}

SceneObject * SceneObject::getRoot()
{
	if (m_parent == nullptr) {
		// If no parent, this is root
		return this;
	} else{
		// Else traverse up tree until root found
		return m_parent->getRoot();
	}
}

std::vector<SceneObject*> SceneObject::getDescendants()
{
	std::vector<SceneObject*> descendants;
	for (SceneObject* child : m_children) {
		// Add child to descendants vector
		descendants.push_back(child);
		// Add child's descendants to descendants vector
		// Recursion stops when a descendant has no children to call this on
		std::vector<SceneObject*> childDescendants = child->getDescendants();
		descendants.insert(descendants.end(), childDescendants.begin(), childDescendants.end());
	}
	return descendants;
}

void SceneObject::notifyCollision(SceneObject * other, Vector2 penetration)
{
	// No default behaviour
}

void SceneObject::notifyOutOfBounds(Vector2 penetration)
{
	// no default behaviour
}

Collider * SceneObject::getCollider()
{
	// Setup collider if it doesn't already exist
	if (m_collider == nullptr) {
		setupCollider();
	}
	return m_collider;
}

void SceneObject::calculateGlobalTransform()
{
	if (m_parent == nullptr) {
		// If this is root, global transform is local transform
		m_globalTransform = m_localTransform;
	} else{
		// Transform localTransform by parent's globalTransform
		m_globalTransform = m_parent->m_globalTransform * m_localTransform;
	}
}

void SceneObject::setupCollider()
{
	// Default behaviour: collider is default AABox
	if (m_collider == nullptr) {
		m_collider = new AABox();
	}
}

void SceneObject::prepareForTransfer(SceneObject * target)
{
	// no default behaviour
}

void SceneObject::performChildTransfer(SceneObject * child, SceneObject * target)
{
	// Check child's parent is this
	if (child->m_parent == this) {
		// Calculate child's new local transform as inverse of target's global transform times child's current global transform 
		Matrix3 newLocalTransform = child->m_globalTransform;
		bool inversionSucceeded = target->m_globalTransform.transformByInverse(newLocalTransform);
		if (inversionSucceeded) {
			// Child alters any internal variables to suit new frame of reference
			child->prepareForTransfer(target);
			// Child's parent reset to nullptr;
			child->m_parent = nullptr;
			// Child's local transform set to newLocalTransform
			child->setLocalTransform(newLocalTransform);
			// Child removed from m_children
			m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
			// Child added to target
			target->addChild(child);
		}
	}
}
