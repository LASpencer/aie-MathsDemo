#pragma once
#include "SceneObject3D.h"

class RocketCockpit;

class Rocket : public SceneObject3D
{
public:
	Rocket();
	~Rocket();

	virtual void update(float deltaTime);

protected:
	Vector4 m_colour;
	Vector3 m_velocity;
	RocketCockpit* m_cockpit;

	void setupChildren();
};

