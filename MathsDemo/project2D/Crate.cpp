#include "Crate.h"
#include "Texture.h"
#include "OBox.h"

const char* Crate::DEFAULT_SPRITE = "./textures/tankBeige.png";

Crate::Crate() : m_sprite(new aie::Texture(DEFAULT_SPRITE))
{
	m_width = m_sprite->getWidth();
	m_height = m_sprite->getHeight();
	m_localTransform.setIdentity();
}

Crate::Crate(Vector2 position, const char * sprite) : m_sprite(new aie::Texture(sprite))
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

	//TODO handle being grabbed and dropped
	//TODO gravity?
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
	if (dynamic_cast<Crate*>(other) != nullptr) {
		// if crate was hit, move back half penetration
		globalTranslate(0.5f * penetration);
	}
	//TODO check if hit by robot hand, be grabbed if it's trying to grab
	//TODO if not held, maybe arm parts push it around?
}

void Crate::notifyOutOfBounds(Vector2 penetration)
{
	// move back into bounds
	globalTranslate(penetration);
}

void Crate::setupCollider()
{
	//TODO OBox around sprite
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	((OBox*)m_collider)->setHalfExtents((Vector2)m_globalTransform[0] * 0.5f*m_width, (Vector2)m_globalTransform[1] * 0.5*m_height);
	((OBox*)m_collider)->setCentre((Vector2)m_globalTransform[2]);
}
