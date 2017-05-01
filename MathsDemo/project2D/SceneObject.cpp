#include "SceneObject.h"

using namespace lasmath;

//TODO commenting

SceneObject::SceneObject() : m_parent(nullptr)
{
	m_localTransform.setIdentity();
}


SceneObject::~SceneObject()
{
	for (auto child : m_children) {
		delete child;
	}
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

void SceneObject::calculateGlobalTransform()
{
	if (m_parent == nullptr) {
		m_globalTransform = m_localTransform;
	} else{
		m_globalTransform = m_parent->m_globalTransform * m_localTransform;
	}
}
