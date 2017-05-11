#include "RocketCockpit.h"
#include "GLMAdaptor.h"

const Vector4 RocketCockpit::COLOUR = {0.8f,0.8f,0,1 };
const float RocketCockpit::RADIUS = 0.1f;
const size_t RocketCockpit::SEGMENTS = 10;

RocketCockpit::RocketCockpit()
{
}


RocketCockpit::~RocketCockpit()
{
}

void RocketCockpit::update(float deltaTime)
{
	mat4 transform = GLMAdaptor::Matrix4Converter(m_globalTransform);
	aie::Gizmos::addSphere(vec3(0), RADIUS, SEGMENTS, SEGMENTS, GLMAdaptor::Vector4Converter(COLOUR), &transform);
	SceneObject3D::update(deltaTime);
}
