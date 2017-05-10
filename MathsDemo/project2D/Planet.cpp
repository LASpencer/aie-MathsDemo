#include "Planet.h"
#include "Gizmos.h"
#include "GLMAdaptor.h"

const int Planet::ROWS = 8;
const int Planet::COLUMNS = 8;

Planet::Planet()
{
}

Planet::Planet(float radius, float distance, float anomaly, float inclination,
	float argument, float rotationRate, float precession) :
	m_radius(radius), m_distance(distance), m_anomaly(anomaly), m_inclination(inclination),
	m_argument(argument), m_rotationRate(rotationRate), m_precession(precession)
{
}


Planet::~Planet()
{
}

void Planet::update(float deltaTime)
{
	Matrix4 position, rotate, flipAxes;
	flipAxes.setEulerRotate(0.0f, -1.57077f, 3.141592f);
	m_anomaly += deltaTime * m_rotationRate;
	m_argument += deltaTime * m_precession;
	rotate.setEulerRotate(m_argument, m_inclination, m_anomaly);
	rotate = flipAxes * rotate;
	//HACK try with flipped axes, see how it looks
	position.setIdentity();
	position[3][0] = m_distance;
	position = rotate * position;
	m_localTransform = position;
	calculateGlobalTransform();
	Vector3 centre = (Vector3)m_globalTransform[3];
	aie::Gizmos::addSphere(GLMAdaptor::Vector3Converter(centre), m_radius,ROWS,COLUMNS, vec4(1, 0, 0, 0.5f));		//TODO replace fillcolour with variable allowing colour to change
	SceneObject3D::update(deltaTime);
}
