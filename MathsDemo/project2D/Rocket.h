#pragma once
#include "SceneObject3D.h"

class RocketCockpit;

// A SceneObject3D controlled by the player which can turn and accelerate
class Rocket : public SceneObject3D
{
public:
	Rocket();
	~Rocket();

	virtual void update(float deltaTime);

	static const float RADIUS;
	static const float HALF_LENGTH;
	static const float TURN_RATE;			// Rate at which rocket pitches and yaws, in radians per second
	static const float SPIN_RATE;			// Rate at which rocket rolls, in radians per second
	static const float ACCELERATION;		// Acceleration in units per second
	static const float MAX_SPEED;			// Maxiumum speed in units per second
	static const size_t SEGMENTS;			// Number of polygons rendered for sides of rocket
	static const Vector4 COLOUR;
	static const int THRUST_KEY;			// Key pressed to accelerate rocket

protected:
	Vector3 m_velocity;
	RocketCockpit* m_cockpit;				// Round cockpit at front of rocket

	void setupChildren();
};

