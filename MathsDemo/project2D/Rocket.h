#pragma once
#include "SceneObject3D.h"

class RocketCockpit;

// A SceneObject3D controlled by the player which can turn and accelerate
class Rocket : public SceneObject3D
{
public:
	Rocket();
	Rocket(Vector3 position);
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
	// input keys
	static const int YAW_LEFT_KEY;			// Yaw to left
	static const int YAW_RIGHT_KEY;			// Yaw to right
	static const int ROLL_LEFT_KEY;			// Roll left
	static const int ROLL_RIGHT_KEY;		// Roll right
	static const int PITCH_UP_KEY;			// Pitch up
	static const int PITCH_DOWN_KEY;		// Pitch down
	static const int THRUST_KEY;			// accelerate rocket

protected:
	Vector3 m_velocity;
	RocketCockpit* m_cockpit;				// Round cockpit at front of rocket

	void setupChildren();
};

