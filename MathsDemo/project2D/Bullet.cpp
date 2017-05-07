#include "Bullet.h"

const float Bullet::DEFAULT_SPEED = 100.0f;
const float Bullet::RADIUS = 5.0f;

Bullet::Bullet() : m_direction(1,0),m_speed(DEFAULT_SPEED)
{
}

Bullet::Bullet(Vector2 position, Vector2 direction, float speed)
{
	bool valid = direction.normalise();
	if (!valid) {
		//TODO throw argument exception
	}
	m_speed = abs(speed);
	m_direction = direction;
	m_localTransform.setIdentity();
	m_localTransform[2] = { position[0],position[1],1 };	
}


Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{
	// move bullet
	Vector2 velocity = deltaTime*m_speed*m_direction;
	m_localTransform[2] = m_localTransform[2] + (Vector3)velocity;
	SceneObject::update(deltaTime);
}

void Bullet::draw(aie::Renderer2D * renderer)
{
	//TODO somewhere, set the render colour appropriately
	renderer->drawCircle(m_globalTransform[2][0], m_globalTransform[2][1], RADIUS);
}

void Bullet::bounce(Vector2 normal)
{
	//TODO bullet bounces off plane given by normal
}

void Bullet::setDirection(Vector2 direction)
{
	bool valid = direction.normalise();
	if (!valid) {
		//TODO throw argument exception
	}
	m_direction = direction;
}

void Bullet::setSpeed(float speed)
{
	m_speed = abs(speed);
}

void Bullet::prepareForTransfer(SceneObject * target)
{
	//TODO change direction to match new parent
}
