#include "SceneObject.h"
#include "Collider.h"
#include "AABox.h"

using namespace lasmath;

//TODO commenting

SceneObject::SceneObject() : m_parent(nullptr)
{
	m_localTransform.setIdentity();
	setupCollider();
}


SceneObject::~SceneObject()
{
	for (auto child : m_children) {
		delete child;
	}
	delete m_collider;
}

void SceneObject::addChild(SceneObject * child)
{
	if (child->m_parent == nullptr) {
		child->m_parent = this;
		m_children.push_back(child);
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

void SceneObject::update(float deltaTime)
{
	
	calculateGlobalTransform();
	setupCollider();
	for (auto child : m_children) {
		child->update(deltaTime);
	}
}

void SceneObject::draw(aie::Renderer2D * renderer)
{
	for (auto child : m_children) {
		child->draw(renderer);
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

void SceneObject::notifyCollision(SceneObject * other)
{
	// No default behaviour
}

Collider * SceneObject::getCollider()
{
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
	if (m_collider = nullptr) {
		m_collider = new AABox();
	}
}
