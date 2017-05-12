#include "TankTurret.h"
#include "Texture.h"
#include "Input.h"
#include "OBox.h"
 
TankTurret::TankTurret() : m_turretSprite(new aie::Texture("./textures/barrelGreen.png"))
{
}


TankTurret::~TankTurret()
{
	delete m_turretSprite;
}

void TankTurret::update(float deltaTime)
{
	SceneObject::update(deltaTime);
}

void TankTurret::draw(aie::Renderer2D * renderer)
{
	// draw turret with origin at bottom of sprite
	renderer->drawSpriteTransformed3x3(m_turretSprite, (float*)m_globalTransform, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f);
	SceneObject::draw(renderer);
}

void TankTurret::notifyCollision(SceneObject * other, Vector2 penetration)
{
	if (other != m_parent && m_parent != nullptr) {
		// If collision wasn't with parent, treat as parent colliding with other object
		m_parent->notifyCollision(other, penetration);
	}
}

void TankTurret::notifyOutOfBounds(Vector2 penetration)
{
	if (m_parent != nullptr) {
		// Treat as parent being out of bounds
		m_parent->notifyOutOfBounds(penetration);
	}
}

void TankTurret::setupCollider()
{
	// Create OBox if collider doesn't exist
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	// Place and orient OBox around sprite
	((OBox*)m_collider)->setHalfExtents((Vector2)m_globalTransform[0] * 8, (Vector2)m_globalTransform[1] * 25);
	((OBox*)m_collider)->setCentre((Vector2)m_globalTransform[2]+ (Vector2)m_globalTransform[1] * 25);
}
