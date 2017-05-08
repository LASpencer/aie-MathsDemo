#include "Bullet.h"
#include "Wall.h"
#include "Obstacle.h"
#include "CircleCollider.h"

const float Bullet::DEFAULT_SPEED = 100.0f;
const float Bullet::RADIUS = 5.0f;

Bullet::Bullet() : m_velocity(DEFAULT_SPEED,0)
{
}

Bullet::Bullet(Vector2 position, Vector2 velocity)
{
	m_velocity = velocity;
	m_localTransform.setIdentity();
	m_localTransform[2] = { position[0],position[1],1 };	
}


Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{
	// move bullet
	Vector2 displacement = deltaTime*m_velocity;
	m_localTransform[2] = m_localTransform[2] + (Vector3)displacement;
	SceneObject::update(deltaTime);
}

void Bullet::draw(aie::Renderer2D * renderer)
{
	//TODO pick a bullet colour
	renderer->setRenderColour(1, 1, 1, 1);
	renderer->drawCircle(m_globalTransform[2][0], m_globalTransform[2][1], RADIUS);
}

void Bullet::bounce(Vector2 normal)
{
	//TODO bullet bounces off plane given by normal
	normal.normalise();
	float projection = m_velocity.dot(normal);
	Vector2 reflection = m_velocity - 2 * projection*normal;
	//TODO inelasticity and friction
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
		// Convert displacement to local coordinates
		if (m_parent != nullptr) {
			Vector3 localDisplacement = (Vector3)displacement;
			if (m_parent->getGlobalTransform().transformByInverse(localDisplacement)) {
				displacement = (Vector2)localDisplacement;
			}
		}
		// add displacement to local translation
		m_localTransform[2] = m_localTransform[2] + (Vector3)displacement;
		// reflect velocity off wall
		bounce(penetration);
	}
	else if (dynamic_cast<Obstacle*>(other) != nullptr) {
		//TODO if obstacle was hit, destroy this and the obstacle
	}
	
	
}


void Bullet::prepareForTransfer(SceneObject * target)
{
	//TODO change direction to match new parent
}

void Bullet::setupCollider()
{
	if (m_collider == nullptr) {
		m_collider = new CircleCollider((Vector2)m_globalTransform[2],RADIUS);
	} else{
		((CircleCollider*)m_collider)->setCentre((Vector2)m_globalTransform[2]);
	}
}
