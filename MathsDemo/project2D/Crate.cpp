#include "Crate.h"
#include "Texture.h"
#include "OBox.h"
#include "RobotHand.h"

const char* Crate::DEFAULT_SPRITE = "./textures/tankBeige.png";

Crate::Crate() : m_sprite(new aie::Texture(DEFAULT_SPRITE)), m_held(false)
{
	m_width = m_sprite->getWidth();
	m_height = m_sprite->getHeight();
	m_localTransform.setIdentity();
}

Crate::Crate(Vector2 position, const char * sprite) : m_sprite(new aie::Texture(sprite)), m_held(false)
{
	m_width = m_sprite->getWidth();
	m_height = m_sprite->getHeight();
	m_localTransform.setIdentity();
	m_localTransform[2] = { position[0],position[1],1 };
	calculateGlobalTransform();
}


Crate::~Crate()
{
}

void Crate::update(float deltaTime)
{
	SceneObject::update(deltaTime);
}

void Crate::draw(aie::Renderer2D * renderer)
{
	// Draw crate
	renderer->setRenderColour(1, 1, 1, 1);
	renderer->drawSpriteTransformed3x3(m_sprite, (float*)m_globalTransform);
	SceneObject::draw(renderer);
}

void Crate::notifyCollision(SceneObject * other, Vector2 penetration)
{
	if (m_held) {
		// if held by robot hand, pass collision on to parent
		if (m_parent != other && dynamic_cast<RobotHand*>(m_parent) != nullptr) {
			m_parent->notifyCollision(other, penetration);
		}
	}
	else {
		if (dynamic_cast<Crate*>(other) != nullptr) {
			if (((Crate*)other)->m_held) {
				// if crate in hand, move back penetration
				globalTranslate(penetration);
			}
			else {
				// if crate is free, move back half penetration
				globalTranslate(0.5f * penetration);
			}
		}
	}
}
void Crate::notifyOutOfBounds(Vector2 penetration)
{
	if (m_held && m_parent != nullptr) {
		// if held, inform parent
		m_parent->notifyOutOfBounds(penetration);
	}
	else {
		// move back into bounds
		globalTranslate(penetration);
	}
}

void Crate::setupCollider()
{
	// Create collider if none exists
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	// Set collider's position and extents to be around sprite
	((OBox*)m_collider)->setHalfExtents((Vector2)m_globalTransform[0] * 0.5f*m_width, (Vector2)m_globalTransform[1] * 0.5*m_height);
	((OBox*)m_collider)->setCentre((Vector2)m_globalTransform[2]);
}
