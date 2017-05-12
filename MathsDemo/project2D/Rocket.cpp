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
	// Add cockpit to front of rocket
	setupChildren();
}

Rocket::Rocket(Vector3 position) : m_velocity({0,0,0})
{
	// rotate so cylinder is facing x axis
	m_localTransform.setRotateZ(1.57077f);
	// set starting position
	m_localTransform[3] += (Vector4)position;
	calculateGlobalTransform();
	// Add cockpit to front of rocket
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
	// Turn ship
	if (input->isKeyDown(aie::INPUT_KEY_A)) {			// Yaw left
		yaw += deltaTime*TURN_RATE;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_D)) {		// Yaw right
		yaw -= deltaTime * TURN_RATE;
	}
	if (input->isKeyDown(aie::INPUT_KEY_Q)) {			// Roll left
		roll -= deltaTime * SPIN_RATE;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_E)) {		// Roll right
		roll += deltaTime * SPIN_RATE;
	}
	if (input->isKeyDown(aie::INPUT_KEY_S)) {			// Pitch up
		pitch += deltaTime * TURN_RATE;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_W)) {		// Pitch down
		pitch -= deltaTime * TURN_RATE;
	}
	// Activate thrust
	if (input->isKeyDown(THRUST_KEY)) {
		//Calculate new velocity
		Vector3 deltaV = { 0, ACCELERATION,0 };		// Cylinder is y-axis aligned
		deltaV = (Vector3)(m_localTransform * (Vector4)deltaV);
		Vector3 newVelocity = m_velocity + deltaV;
		// If new velocity less than max speed, set as velocity
		if (newVelocity.compareMagnitude(MAX_SPEED) == -1) {
			m_velocity = newVelocity;
		}
	}
	// Rotate to new orientation
	turn.setTaitBryanRotate(yaw, roll,pitch);
	m_localTransform = m_localTransform * turn;
	// Move by velocity
	m_localTransform[3] += (Vector4)m_velocity;
	// Calculate global transform
	SceneObject3D::update(deltaTime);
	// Add cylinder to Gizmos
	mat4 transformation = GLMAdaptor::Matrix4Converter(m_globalTransform);
	aie::Gizmos::addCylinderFilled(vec3(0), RADIUS, HALF_LENGTH, SEGMENTS, GLMAdaptor::Vector4Converter(COLOUR), &transformation);
	
}

void Rocket::setupChildren()
{
	m_cockpit = new RocketCockpit();
	// Place m_cockpit at front of rocket
	Matrix4 cockpitPosition;
	cockpitPosition.setIdentity();
	cockpitPosition[3][1] = HALF_LENGTH;		//cylinder aligned with Y axis
	m_cockpit->setLocalTransform(cockpitPosition);
	addChild(m_cockpit);	
}
