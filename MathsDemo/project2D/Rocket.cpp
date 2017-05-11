#include "Rocket.h"
#include "Input.h"
#include "Gizmos.h"
#include "GLMAdaptor.h"
#include "RocketCockpit.h"

const float Rocket::RADIUS = 0.1f;
const float Rocket::HALF_LENGTH = 0.2f;
const float Rocket::TURN_RATE = 1;
const float Rocket::SPIN_RATE = 2;
const float Rocket::ACCELERATION = 0.001f;
const float Rocket::MAX_SPEED = 0.05f;
const size_t Rocket::SEGMENTS = 10;
const Vector4 Rocket::COLOUR = {0,0.6f,1,1};

const int Rocket::THRUST_KEY = aie::INPUT_KEY_SPACE;

Rocket::Rocket() : m_velocity({0,0,0})
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
	//TODO make input keys constants
	if (input->isKeyDown(aie::INPUT_KEY_A)) {
		yaw += deltaTime*TURN_RATE;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_D)) {
		yaw -= deltaTime * TURN_RATE;
	}
	if (input->isKeyDown(aie::INPUT_KEY_Q)) {
		roll -= deltaTime * SPIN_RATE;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_E)) {
		roll += deltaTime * SPIN_RATE;
	}
	if (input->isKeyDown(aie::INPUT_KEY_S)) {
		pitch += deltaTime * TURN_RATE;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_W)) {
		pitch -= deltaTime * TURN_RATE;
	}
	if (input->isKeyDown(THRUST_KEY)) {
		//Calculate change in velocity
		Vector3 deltaV = { 0, ACCELERATION,0 };		// Cylinder is y-axis aligned
		deltaV = (Vector3)(m_localTransform * (Vector4)deltaV);
		Vector3 newVelocity = m_velocity + deltaV;
		// If new velocity less than max speed, set as velocity
		if (newVelocity.compareMagnitude(MAX_SPEED) == -1) {
			m_velocity = newVelocity;
		}
	}
	turn.setTaitBryanRotate(yaw, roll,pitch);
	m_localTransform = m_localTransform * turn;
	//TODO move by velocity
	m_localTransform[3] += (Vector4)m_velocity;
	mat4 transformation = GLMAdaptor::Matrix4Converter(m_globalTransform);
	aie::Gizmos::addCylinderFilled(vec3(0), RADIUS, HALF_LENGTH, SEGMENTS, GLMAdaptor::Vector4Converter(COLOUR), &transformation);
	SceneObject3D::update(deltaTime);
}

void Rocket::setupChildren()
{
	m_cockpit = new RocketCockpit();	//TODO use constant instead of magic numbers
	Matrix4 cockpitPosition;
	cockpitPosition.setIdentity();
	cockpitPosition[3][1] = HALF_LENGTH;
	m_cockpit->setLocalTransform(cockpitPosition);
	addChild(m_cockpit);	
}
