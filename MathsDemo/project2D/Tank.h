#pragma once
#include "SceneObject.h"

class TankTurret;

class Tank :
	public SceneObject
{
public:
	Tank();
	Tank(Vector2 position);
	~Tank();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	void fireBullet();

	virtual void notifyCollision(SceneObject* other, Vector2 penetration);

	virtual void notifyOutOfBounds(Vector2 penetration);

	static const int LEFT_TURN_KEY;
	static const int RIGHT_TURN_KEY;
	static const int FORWARD_KEY;
	static const int REVERSE_KEY;
	static const int FIRE_KEY;
	static const int LEFT_AIM_KEY;
	static const int RIGHT_AIM_KEY;

	static const float AIM_RATE;
	static const float TURN_RATE;
	static const float FORWARD_SPEED;
	static const float REVERSE_SPEED;
	static const float BULLET_COOLDOWN;
	static const float BARREL_LENGTH;


protected:
	TankTurret* m_turret;
	aie::Texture* m_tankSprite;

	float m_bulletCD;

	void setupCollider();
};

