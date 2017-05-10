#include "SceneObject3D.h"

using namespace lasmath;

SceneObject3D::SceneObject3D() : m_parent(nullptr), m_childrenLocked(false)
{
	m_localTransform.setIdentity();
	calculateGlobalTransform();
}


SceneObject3D::~SceneObject3D()
{
	m_childrenLocked = true;
	for (auto child : m_children) {
		delete child;
	}
	m_childrenLocked = false;
}

bool SceneObject3D::addChild(SceneObject3D * child)
{
	if (child->m_parent == nullptr) {
		if (!m_childrenLocked) {
			child->m_parent = this;
			m_children.push_back(child);
		} else{
			m_toAdd.push_back(child);
		}
		return true;
	} else{
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
	calculateGlobalTransform();
	m_childrenLocked = true;
	for (auto child : m_children) {
		child->update(deltaTime);
	}
	m_childrenLocked = false;
}

void SceneObject3D::updateChildList()
{
	m_childrenLocked = true;
	for (auto child : m_children) {
		child->updateChildList();
	}
	m_childrenLocked = false;
	for (auto child : m_toAdd) {
		addChild(child);
	}
	m_toAdd.clear();
}

std::vector<SceneObject3D*> SceneObject3D::getDescendants()
{
	std::vector<SceneObject3D*> descendants;
	for (SceneObject3D* child : m_children) {
		descendants.push_back(child);
		std::vector<SceneObject3D*> childDescendants = child->getDescendants();
		descendants.insert(descendants.end(), childDescendants.begin(), childDescendants.end());
	}
	return descendants;
}

void SceneObject3D::calculateGlobalTransform()
{
	if (m_parent == nullptr) {
		m_globalTransform = m_localTransform;
	}
	else {
		m_globalTransform = m_parent->m_globalTransform * m_localTransform;
	}
}
