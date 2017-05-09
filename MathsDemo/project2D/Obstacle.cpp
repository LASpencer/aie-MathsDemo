#include "Obstacle.h"
#include "Texture.h"
#include "Wall.h"
#include "Tank.h"
#include "TankTurret.h"
#include "Bullet.h"
#include "OBox.h"

const char*  Obstacle::DEFAULT_SPRITE = "./textures/rock_large.png";
const int Obstacle::DEF_HEALTH = 1;

Obstacle::Obstacle() : m_health(DEF_HEALTH), m_sprite(new aie::Texture(DEFAULT_SPRITE)), m_dead(false)
{
	m_width = m_sprite->getWidth();
	m_height = m_sprite->getHeight();
	m_localTransform.setIdentity();
}

Obstacle::Obstacle(Vector2 position, const char * sprite, int health) : m_health(health), m_sprite(new aie::Texture(sprite)), m_dead(false)
{
	m_width = m_sprite->getWidth();
	m_height = m_sprite->getHeight();
	m_localTransform.setIdentity();
	m_localTransform[2] = { position[0],position[1],1 };
	calculateGlobalTransform();
}


Obstacle::~Obstacle()
{
	delete m_sprite;
}

void Obstacle::update(float deltaTime)
{
	// Check if dead, and if so request destruction
	if (m_health <= 0) {
		m_dead = true;
	}
	if (m_dead && m_parent!=nullptr) {
		m_parent->removeChild(this);
	}
	SceneObject::update(deltaTime);
}

void Obstacle::draw(aie::Renderer2D * renderer)
{
	// Draw obstacle
	renderer->setRenderColour(1, 1, 1, 1);
	renderer->drawSpriteTransformed3x3(m_sprite, (float*)m_globalTransform);
	SceneObject::draw(renderer);
}

void Obstacle::notifyCollision(SceneObject * other, Vector2 penetration)
{
	if (dynamic_cast<Wall*>(other) != nullptr) {
		// if wall was hit, move back penetration
		globalTranslate(penetration);
	}
	else if (dynamic_cast<Tank*>(other) != nullptr) {
		//if tank was hit, pushed back half penetration
		globalTranslate(0.5*penetration);
	}else if (dynamic_cast<TankTurret*>(other) != nullptr) {
		//if tank turret was hit, pushed back half penetration
		globalTranslate(0.5*penetration);
	}
	else if (dynamic_cast<Obstacle*>(other) != nullptr) {
		//if obstacle was hit, pushed back half penetration
		globalTranslate(0.5*penetration);
	}
	else if (dynamic_cast<Bullet*>(other)!=nullptr){
		//if bullet was hit, take damage
		--m_health;
	}
}

void Obstacle::notifyOutOfBounds(Vector2 penetration)
{
	// move back into bounds
	globalTranslate(penetration);
}

void Obstacle::setupCollider()
{
	//TODO OBox around sprite
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	((OBox*)m_collider)->setHalfExtents((Vector2)m_globalTransform[0] * 0.5f*m_width, (Vector2)m_globalTransform[1] * 0.5*m_height);
	((OBox*)m_collider)->setCentre((Vector2)m_globalTransform[2]);
}
