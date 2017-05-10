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
	m_childrenLocked = true;
	for (auto child : m_children) {
		delete child;
	}
	m_childrenLocked = false;
	delete m_collider;
}

bool SceneObject::addChild(SceneObject * child)
{
	if (child->m_parent == nullptr) {
		if (!m_childrenLocked) {
			child->m_parent = this;
			m_children.push_back(child);
		}
		else {
			m_toAdd.push_back(child);
		}
		return true;
	}
	else {
		return false;
	}
}

bool SceneObject::removeChild(SceneObject * child)
{
	//TODO check if child, or its children, are marked as targets or source for transfer
	if (child->safeToDelete()) {
		if (!m_childrenLocked) {
			m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
			delete child;
		}
		else {
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
	//TODO check if child, or target, are marked for deletion, or child already marked for transfer
	if (child->safeForTransfer() && !target->markedForDeletion() && child != target) {
		std::pair<SceneObject*, SceneObject*> arguments = std::make_pair(child, target);
		m_toTransfer.push_back(arguments);
		// flag child as to be transferred
		child->m_transferFlag = true;
		target->m_transferTargetFlag = true;
		//TODO return true if transfer is valid
		return true;
	}
	else {
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
	Matrix3 rotation;
	rotation.setRotateZ(angle);
	m_localTransform = m_localTransform * rotation;
}

void SceneObject::scale(const Vector2 & proportions)
{
	Matrix3 scale = { proportions[0],0,0,0,proportions[1],0,0,0,1 };
	m_localTransform = m_localTransform * scale;
}

void SceneObject::translate(const Vector2 & vec)
{
	Matrix3 translate;
	translate.setIdentity();
	translate[2] = { vec[0],vec[1],1 };
	m_localTransform = m_localTransform * translate;
}

void SceneObject::transform(const Matrix3 & transformation)
{
	m_localTransform = m_localTransform * transformation;
}

void SceneObject::globalTranslate(const Vector2 & vec)
{
	Vector2 translation = vec;
	if (m_parent != nullptr) {
		Vector3 localTranslation = (Vector3)translation;
		m_parent->getGlobalTransform().transformByInverse(localTranslation);
		translation = (Vector2)localTranslation;
	}
	m_localTransform[2] = m_localTransform[2] + (Vector3)translation;
}

void SceneObject::update(float deltaTime)
{
	calculateGlobalTransform();
	setupCollider();
	m_childrenLocked = true;
	for (auto child : m_children) {
		child->update(deltaTime);
	}
	m_childrenLocked = false;
}

void SceneObject::draw(aie::Renderer2D * renderer)
{
	for (auto child : m_children) {
		child->draw(renderer);
	}
}

void SceneObject::updateChildList()
{
	//TODO figure out timing for children to perform additions/deletion/transfers, resetting flags
	if (m_childrenLocked) {
		//TODO throw exception
	}
	m_childrenLocked = true;
	for (auto child : m_children) {
		child->updateChildList();
	}
	m_childrenLocked = false;
	for (auto arguments : m_toTransfer) {
		performChildTransfer(arguments.first, arguments.second);
	}
	m_toTransfer.clear();
	for (auto child : m_toAdd) {
		addChild(child);
	}
	m_toAdd.clear();
	for (auto child : m_toDelete) {
		removeChild(child);
	}
	m_toDelete.clear();
	m_deletionFlag = false;
	m_transferFlag = false;
	m_transferTargetFlag = false;
}

bool SceneObject::safeToDelete()
{
	// Safe to delete if not marked to be tranferred or as target of transfer
	bool safeToDelete = !(m_transferFlag || m_transferTargetFlag);
	// If any child not safe to delete, this is not safe to delete
	for (auto child : m_children) {
		if (!child->safeToDelete()) {
			safeToDelete = false;
			break;
		}
	}
	return safeToDelete;
}

bool SceneObject::markedForDeletion()
{
	// Marked for deletion if this or any parent has m_deletionFlag true
	bool marked = m_deletionFlag;
	if (m_parent != nullptr) {
		if (m_parent->markedForDeletion()) {
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
		return this;
	} else{
		return m_parent->getRoot();
	}
}

std::vector<SceneObject*> SceneObject::getDescendants()
{
	std::vector<SceneObject*> descendants;
	for (SceneObject* child : m_children) {
		descendants.push_back(child);
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
	if (m_collider == nullptr) {
		setupCollider();
	}
	return m_collider;
}

void SceneObject::calculateGlobalTransform()
{
	if (m_parent == nullptr) {
		m_globalTransform = m_localTransform;
	} else{
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
			// Child removed from m_children
			m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
			// Child added to target
			target->addChild(child);
		}
		else {
			//TODO decide behaviour on failed transfer
		}
	}
}
