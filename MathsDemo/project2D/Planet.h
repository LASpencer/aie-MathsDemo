#pragma once
#include "SceneObject3D.h"

class Planet : public SceneObject3D
{
public:
	Planet();
	Planet(float radius, float distance, float anomaly, float inclination,
		float argument, float rotationRate, float precession);
	~Planet();

	
protected:
	float m_distance;		//distance from parent object
	float m_anomaly;		//current rotation along orbit
	float m_rotationRate;	//change in anomaly per second
	float m_inclination;	//tilt of orbit relative to plane
	float m_argument;		//angle where orbit intersects with plane
	float m_precession;		//change in argument per second
	float m_radius;			//radius of sphere drawn
};

