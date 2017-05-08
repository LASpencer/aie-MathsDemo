#pragma once
#include "SceneObject.h"
class Bullet :
	public SceneObject
{
public:
	Bullet();
	Bullet(Vector2 position, Vector2 velocity);
	~Bullet();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	void bounce(Vector2 normal);

	//TODO set/get direction, speed
	void setVelocity(Vector2 velocity);

	Vector2 getVelocity(){
		return m_velocity;
	}

	virtual void notifyCollision(SceneObject* other, Vector2 penetration);

	static const float DEFAULT_SPEED;
	static const float RADIUS;

protected:
	Vector2 m_velocity;

	void prepareForTransfer(SceneObject* target);

	virtual void setupCollider();
};

