#include "RocketCockpit.h"
#include "GLMAdaptor.h"


RocketCockpit::RocketCockpit()
{
}

RocketCockpit::RocketCockpit(float radius, Vector4 colour) : m_radius(radius), m_colour(colour)
{
}


RocketCockpit::~RocketCockpit()
{
}

void RocketCockpit::update(float deltaTime)
{
	mat4 transform = GLMAdaptor::Matrix4Converter(m_globalTransform);
	aie::Gizmos::addSphere(vec3(0), m_radius, 4, 4, GLMAdaptor::Vector4Converter(m_colour), &transform);
	SceneObject3D::update(deltaTime);
}
