#include "Bullet.h"
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
}

void Bullet::setVelocity(Vector2 velocity)
{
	m_velocity = velocity;
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
