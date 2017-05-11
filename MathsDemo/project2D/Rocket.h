#pragma once
#include "SceneObject3D.h"

class RocketCockpit;

class Rocket : public SceneObject3D
{
public:
	Rocket();
	~Rocket();

	virtual void update(float deltaTime);

	static const float RADIUS;
	static const float HALF_LENGTH;
	static const float TURN_RATE;
	static const float SPIN_RATE;
	static const float ACCELERATION;
	static const float MAX_SPEED;
	static const size_t SEGMENTS;
	static const Vector4 COLOUR;
	static const int THRUST_KEY;

protected:
	Vector3 m_velocity;
	RocketCockpit* m_cockpit;

	void setupChildren();
};

