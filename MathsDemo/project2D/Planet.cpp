#include "Planet.h"
#include "Gizmos.h"
#include "GLMAdaptor.h"

const int Planet::ROWS = 8;
const int Planet::COLUMNS = 8;

const Vector4  Planet::DEF_FILL_COLOUR = {1,0,0,0.8f};

Planet::Planet() : m_radius(1), m_distance(0), m_rotationRate(0), m_anomaly(0),m_inclination(0),m_argument(0), m_precession(0), m_colour(DEF_FILL_COLOUR)
{
}

Planet::Planet(float radius, float distance, float rotationRate, float anomaly, float inclination,
	float argument, float precession, Vector4 colour) :
	m_radius(radius), m_distance(distance), m_rotationRate(rotationRate), m_anomaly(anomaly), m_inclination(inclination),
	m_argument(argument), m_precession(precession), m_colour(colour)
{
}


Planet::~Planet()
{
}

void Planet::update(float deltaTime)
{
	Matrix4 position, rotate;
	// Move position along the orbit
	m_anomaly += deltaTime * m_rotationRate;
	// Shift orientation of orbit by precession
	m_argument += deltaTime * m_precession;
	// Calculate orientation
	rotate.setEulerRotate(m_argument, m_inclination, m_anomaly);
	// Move out to distance
	position.setIdentity();
	position[3][0] = m_distance;
	position = rotate * position;
	m_localTransform = position;
	// calculate global transform
	SceneObject3D::update(deltaTime);
	// Add sphere to Gizmos
	mat4 transform = GLMAdaptor::Matrix4Converter(m_globalTransform);
	aie::Gizmos::addSphere(vec3(0), m_radius,ROWS,COLUMNS, GLMAdaptor::Vector4Converter(m_colour), &transform);

}
