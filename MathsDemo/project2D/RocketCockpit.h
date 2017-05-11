#pragma once
#include "SceneObject3D.h"
class RocketCockpit :
	public SceneObject3D
{
public:
	RocketCockpit();
	RocketCockpit(float radius, Vector4 colour);
	~RocketCockpit();

	virtual void update(float deltaTime);

	// Getter/Setter methods
	float getRadius() {
		return m_radius;
	}
	Vector4 getColour() {
		return m_colour;
	}

	void setRadius(float radius) {
		m_radius = radius;
	}
	void setColour(Vector4 colour) {
		m_colour = colour;
	}

protected:
	float m_radius;			//radius of sphere drawn
	Vector4 m_colour;		//colour of sphere
};

