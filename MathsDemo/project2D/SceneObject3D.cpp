#include "SceneObject3D.h"

using namespace lasmath;

SceneObject3D::SceneObject3D() : m_parent(nullptr), m_childrenLocked(false)
{
	m_localTransform.setIdentity();
	calculateGlobalTransform();
}


SceneObject3D::~SceneObject3D()
{
	// Destroy all children
	m_childrenLocked = true;
	for (auto child : m_children) {
		delete child;
	}
	m_childrenLocked = false;
}

bool SceneObject3D::addChild(SceneObject3D * child)
{
	// Check if child already has a parent
	if (child->m_parent == nullptr && child != this) {
		// Check if modifying m_children is safe
		if (!m_childrenLocked) {
			// If safe, add child to m_children and set this as its parent
			child->m_parent = this;
			m_children.push_back(child);
		} else{
			// If not safe, queue child for addition later
			m_toAdd.push_back(child);
		}
		return true;
	} else{
		// If child has a parent, adding child fails
		return false;
	}
}

void SceneObject3D::setLocalTransform(const Matrix4 & transformation)
{
	m_localTransform = transformation;
}

Matrix4 SceneObject3D::getLocalTransform()
{
	return m_localTransform;
}

Matrix4 SceneObject3D::getGlobalTransform()
{
	return m_globalTransform;
}

void SceneObject3D::update(float deltaTime)
{
	// Recalculate global transform
	calculateGlobalTransform();
	// lock m_children before iterating over it
	m_childrenLocked = true;
	// Call updates on all children
	for (auto child : m_children) {
		child->update(deltaTime);
	}
	// unlock m_children after iteration is over
	m_childrenLocked = false;
	if (m_parent == nullptr) {
		// If this is the root, make queued changes to childList
		updateChildList();
	}
}

void SceneObject3D::updateChildList()
{
	// Have all children update their list of children
	m_childrenLocked = true;
	for (auto child : m_children) {
		child->updateChildList();
	}
	m_childrenLocked = false;
	// Add all SceneObjects in toAdd list
	for (auto child : m_toAdd) {
		addChild(child);
	}
	// Clear toAdd list
	m_toAdd.clear();
}

std::vector<SceneObject3D*> SceneObject3D::getDescendants()
{
	std::vector<SceneObject3D*> descendants;
	for (SceneObject3D* child : m_children) {
		// Add child to descendants vector
		descendants.push_back(child);
		// Add child's descendants to descendants vector
		// Recursion stops when a descendant has no children to call getDescendants on
		std::vector<SceneObject3D*> childDescendants = child->getDescendants();
		descendants.insert(descendants.end(), childDescendants.begin(), childDescendants.end());
	}
	return descendants;
}

void SceneObject3D::calculateGlobalTransform()
{
	// Check if this is at the root
	if (m_parent == nullptr) {
		// Root SceneObject3D's globalTransform is localTransform
		m_globalTransform = m_localTransform;
	}
	else {
		// Transform localTransform by parent's globalTransform
		m_globalTransform = m_parent->m_globalTransform * m_localTransform;
	}
}
