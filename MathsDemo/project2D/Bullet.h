#pragma once
#include "SceneObject.h"

// A SceneObject that moves across the screen, bounces off Walls, and damages Obstacles
class Bullet :
	public SceneObject
{
public:
	Bullet();
	Bullet(Vector2 position, Vector2 velocity);
	~Bullet();

	virtual void update(float deltaTime);

	// Draw a circle at the bullet's position
	virtual void draw(aie::Renderer2D* renderer);

	// Bounce bullet off a surface orthogonal to normal
	void bounce(Vector2 normal);

	//Accessor and Mutator methods
	void setVelocity(Vector2 velocity);

	Vector2 getVelocity(){
		return m_velocity;
	}
	
	virtual void notifyCollision(SceneObject* other, Vector2 penetration);

	// Bullet checks if penetration puts it entirely out of bounds and if so is destroyed
	virtual void notifyOutOfBounds(Vector2 penetration);

	static const float DEFAULT_SPEED;
	static const float RADIUS;

protected:
	Vector2 m_velocity;		// Change in bullet's position per second
	bool m_dead;			// flag for whether bullet should be destroyed

	// Create a CircleCollider around bullet
	virtual void setupCollider();
};

