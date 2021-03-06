#include "Bullet.h"
#include "Wall.h"
#include "Obstacle.h"
#include "CircleCollider.h"

const float Bullet::DEFAULT_SPEED = 150.0f;
const float Bullet::RADIUS = 5.0f;

Bullet::Bullet() : m_velocity(DEFAULT_SPEED,0), m_dead(false)
{
}

Bullet::Bullet(Vector2 position, Vector2 velocity) : m_dead(false)
{
	m_velocity = velocity;
	m_localTransform.setIdentity();
	m_localTransform[2] = { position[0],position[1],1 };	
	calculateGlobalTransform();
}


Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{
	// move bullet
	Vector2 displacement = deltaTime*m_velocity;
	m_localTransform[2] = m_localTransform[2] + (Vector3)displacement;
	// If bullet is dead, try removing it from scene graph
	if (m_dead && m_parent != nullptr) {
		m_parent->removeChild(this);
	}
	SceneObject::update(deltaTime);
}

void Bullet::draw(aie::Renderer2D * renderer)
{
	// Draw white circle at bullet's position
	renderer->setRenderColour(1, 1, 1, 1);
	renderer->drawCircle(m_globalTransform[2][0], m_globalTransform[2][1], RADIUS);
}

void Bullet::bounce(Vector2 normal)
{
	//bullet bounces off plane given by normal
	normal.normalise();
	float projection = m_velocity.dot(normal);
	Vector2 reflection = m_velocity - 2 * projection*normal;
	m_velocity = reflection;
}

void Bullet::setVelocity(Vector2 velocity)
{
	m_velocity = velocity;
}

void Bullet::notifyCollision(SceneObject * other, Vector2 penetration)
{
	//if wall was hit, bounce off the wall
	if (dynamic_cast<Wall*>(other) != nullptr) {
		// move back to edge of wall
		Vector2 direction = m_velocity;
		direction.normalise();
		float distance = direction.dot(penetration);
		Vector2 displacement = direction * distance;
		globalTranslate(displacement);
		// reflect velocity by normal of wall
		bounce(penetration);
	}
	else if (dynamic_cast<Obstacle*>(other) != nullptr) {
		//if obstacle was hit, destroy bullet
		m_dead = true;
	}
	
	
}

void Bullet::notifyOutOfBounds(Vector2 penetration)
{
	//if bullet entirely out of bounds, destroy bullet
	if (penetration.compareMagnitude(2 * RADIUS) == 1) {
		m_dead = true;
	}
}


void Bullet::setupCollider()
{
	if (m_collider == nullptr) {
		// If no collider, create new CircleCollider
		m_collider = new CircleCollider((Vector2)m_globalTransform[2],RADIUS);
	} else{
		// Move CircleCollider to bullets position
		((CircleCollider*)m_collider)->setCentre((Vector2)m_globalTransform[2]);
	}
}
