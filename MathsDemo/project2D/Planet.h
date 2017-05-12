#pragma once
#include "SceneObject3D.h"

// A sphere orbiting around its root based on orbital parameters passed to it
class Planet : public SceneObject3D
{
public:
	Planet();
	Planet(float radius, float distance, float rotationRate, float anomaly = 0.0f, float inclination = 0.0f,
		float argument=0.0f, float precession = 0.0f, Vector4 colour = DEF_FILL_COLOUR);
	~Planet();

	virtual void update(float deltaTime);


	// Getter/Setter methods
	float getRadius() {
		return m_radius;
	}
	float getDistance() {
		return m_distance;
	}
	float getAnomaly() {
		return m_anomaly;
	}
	float getRotationRate() {
		return m_rotationRate;
	}
	float getInclination() {
		return m_inclination;
	}
	float getArgument() {
		return m_argument;
	}
	float getPrecession() {
		return m_precession;
	}
	Vector4 getColour() {
		return m_colour;
	}

	void setRadius(float radius) {
		m_radius = radius;
	}
	void setDistance(float distance) {
		m_distance = distance;
	}
	void setAnomaly(float anomaly) {
		m_anomaly = anomaly;
	}
	void setRotationRate(float rotationRate) {
		m_rotationRate = rotationRate;
	}
	void setInclination(float inclination) {
		m_inclination = inclination;
	}
	void setArgument(float argument) {
		m_argument = argument;
	}
	void setPrecession(float precession) {
		m_precession = precession;
	}

	void setColour(Vector4 colour) {
		m_colour = colour;
	}

	static const int ROWS;						// Rows of polygons for rendering sphere
	static const int COLUMNS;					// Columns of polygons for rendering sphere
	static const Vector4 DEF_FILL_COLOUR;		// Default colour for planets

protected:
	float m_distance;		//distance from parent object
	float m_anomaly;		//current rotation along orbit
	float m_rotationRate;	//change in anomaly per second
	float m_inclination;	//tilt of orbit relative to plane
	float m_argument;		//angle where orbit intersects with plane
	float m_precession;		//change in argument per second
	float m_radius;			//radius of sphere drawn
	Vector4 m_colour;		//colour of planet
};

