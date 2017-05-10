#include "Planet.h"



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
