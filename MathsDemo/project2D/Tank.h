#pragma once
#include "SceneObject.h"

class TankTurret;

// A SceneObject controlled by the player which can fire bullets
class Tank :
	public SceneObject
{
public:
	Tank();
	Tank(Vector2 position);
	~Tank();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	// Spawn a bullet at the end of the tank's turret
	void fireBullet();

	// Move away from object hit
	virtual void notifyCollision(SceneObject* other, Vector2 penetration);
	// Move tank by penetration so it's back in bounds
	virtual void notifyOutOfBounds(Vector2 penetration);

	// Input keys for controlling tank
	static const int LEFT_TURN_KEY;		// Turn tank left
	static const int RIGHT_TURN_KEY;	// Turn tank right
	static const int FORWARD_KEY;		// Move tank forwards
	static const int REVERSE_KEY;		// Move tank backwards
	static const int FIRE_KEY;			// Fire a bullet
	static const int LEFT_AIM_KEY;		// Turn turret left
	static const int RIGHT_AIM_KEY;		// Turn turret right

	static const float AIM_RATE;		// Turret turning rate in radians per second
	static const float TURN_RATE;		// Tank turning rate in radians per second
	static const float FORWARD_SPEED;	// Forward movement speed
	static const float REVERSE_SPEED;	// Backward movement speed
	static const float BULLET_COOLDOWN;	// Time between bullet firings
	static const float BARREL_LENGTH;	// Distance from tank centre to spawn bullet


protected:
	TankTurret* m_turret;				// Tank's turret
	aie::Texture* m_tankSprite;

	float m_bulletCD;					// Time until another bullet can be fired

	// Create OBox around tank sprite
	void setupCollider();
};

