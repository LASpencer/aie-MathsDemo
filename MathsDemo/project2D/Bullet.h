#pragma once
#include "SceneObject.h"
class Bullet :
	public SceneObject
{
public:
	Bullet();
	Bullet(Vector2 position, Vector2 direction, float speed = DEFAULT_SPEED);
	~Bullet();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	void bounce(Vector2 normal);

	//TODO set/get direction, speed
	void setDirection(Vector2 direction);

	Vector2 getDirection(){
		return m_direction;
	}

	void setSpeed(float speed);
	
	float getSpeed() {
		return m_speed;
	}

	static const float DEFAULT_SPEED;
	static const float RADIUS;

protected:
	Vector2 m_direction;
	float m_speed;

	void prepareForTransfer(SceneObject* target);
};

