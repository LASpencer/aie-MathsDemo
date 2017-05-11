#include "Planet.h"
#include "Gizmos.h"
#include "GLMAdaptor.h"

const int Planet::ROWS = 8;
const int Planet::COLUMNS = 8;

const Vector4  Planet::DEF_FILL_COLOUR = {1,0,0,0.5f};

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
	m_anomaly += deltaTime * m_rotationRate;
	m_argument += deltaTime * m_precession;
	rotate.setEulerRotate(m_argument, m_inclination, m_anomaly);
	position.setIdentity();
	position[3][0] = m_distance;
	position = rotate * position;
	m_localTransform = position;
	calculateGlobalTransform();
	Vector3 centre = (Vector3)m_globalTransform[3];
	//TODO transform matrix spinning the sphere
	aie::Gizmos::addSphere(GLMAdaptor::Vector3Converter(centre), m_radius,ROWS,COLUMNS, GLMAdaptor::Vector4Converter(m_colour));
	SceneObject3D::update(deltaTime);
}
