#include "Rocket.h"
#include "Input.h"
#include "Gizmos.h"
#include "GLMAdaptor.h"
#include "RocketCockpit.h"

Rocket::Rocket()
{
	// rotate so cylinder is facing x axis
	m_localTransform.setRotateZ(1.57077f);
	calculateGlobalTransform();
	setupChildren();
}


Rocket::~Rocket()
{
}

void Rocket::update(float deltaTime)
{
	float yaw = 0;
	float pitch = 0;
	float roll = 0;
	Matrix4 turn;
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_A)) {
		yaw += deltaTime;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_D)) {
		yaw -= deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_Q)) {
		roll -= deltaTime;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_E)) {
		roll += deltaTime;
	}
	if (input->isKeyDown(aie::INPUT_KEY_S)) {
		pitch += deltaTime;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_W)) {
		pitch -= deltaTime;
	}
	turn.setTaitBryanRotate(yaw, roll,pitch);
	m_localTransform = m_localTransform * turn;
	//TODO move by velocity
	//TODO add acceleration (transformed by matrix) to velocity
	mat4 transformation = GLMAdaptor::Matrix4Converter(m_globalTransform);
	aie::Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &transformation);//TODO use constants/variables instead of literals
	SceneObject3D::update(deltaTime);
}

void Rocket::setupChildren()
{
	m_cockpit = new RocketCockpit(0.6f, { 1,1,0,1 });	//TODO use constant instead of magic numbers
	Matrix4 cockpitPosition;
	cockpitPosition.setIdentity();
	cockpitPosition[3][1] = 1;
	m_cockpit->setLocalTransform(cockpitPosition);
	addChild(m_cockpit);	
}
