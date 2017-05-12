#pragma once
#include "SceneObject.h"

// SceneObject that can be damaged by Bullets
class Obstacle :
	public SceneObject
{
public:
	Obstacle();
	Obstacle(Vector2 position, const char* sprite = DEFAULT_SPRITE, int health = DEF_HEALTH);
	~Obstacle();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	// Move Obstacle away from colliding object, or lower its health if colliding with bullet
	virtual void notifyCollision(SceneObject* other, Vector2 penetration);
	// Move Obstacle by penetration so it's back in bounds
	virtual void notifyOutOfBounds(Vector2 penetration);

	static const int DEF_HEALTH;
	static const char* DEFAULT_SPRITE;

protected:
	int m_health;
	bool m_dead;
	aie::Texture* m_sprite;
	float m_width, m_height;
	virtual void setupCollider();
};

