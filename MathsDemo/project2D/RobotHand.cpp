#include "RobotHand.h"
#include "Texture.h"
#include "Crate.h"
#include "OBox.h"

RobotHand::RobotHand() : m_sprite(nullptr), m_state(RobotHandState::EMPTY)
{
}

RobotHand::RobotHand(aie::Texture * sprite) : m_sprite(sprite), m_state(RobotHandState::EMPTY)
{
}


RobotHand::~RobotHand()
{
	delete m_sprite;
}

void RobotHand::update(float deltaTime)
{
	if (m_state == RobotHandState::RELEASING) {
		releaseCrate();
	}
	SceneObject::update(deltaTime);
}

void RobotHand::draw(aie::Renderer2D * renderer)
{
	if (m_sprite != nullptr) {
		renderer->drawSpriteTransformed3x3(m_sprite, (float*)m_globalTransform, 0.0f, 0.0f, 0.0f, 0.5f, 0.05f);
	}
	SceneObject::draw(renderer);
}

void RobotHand::setState(RobotHandState state)
{
	//TODO check valid transition (Empty <-> Grabbing -> Holding ->Releasing->Empty)
	m_state = state;
}

void RobotHand::notifyCollision(SceneObject * other, Vector2 penetration)
{
	if (dynamic_cast<Crate*>(other) != nullptr) {
		// If collided with a 
		if (m_state == RobotHandState::GRABBING) {
			// try to transfer from other's parent to this (if other is not root or held by a hand)
			if (other->getParent() != nullptr && !((Crate*)other)->isHeld()) {
				bool canGrab = other->getParent()->transferChild(other, this);
				if (canGrab) {
					((Crate*)other)->setHeld(true);
					m_heldCrate = (Crate*)other;
					setState(RobotHandState::HOLDING);
				}
			}
		}
	}
}

void RobotHand::releaseCrate()
{
	// try transferring crate to rool
	SceneObject* root = getRoot();
	bool canRelease = transferChild(m_heldCrate, root);
	if (canRelease) {
		m_heldCrate->setHeld(false);
		m_heldCrate = nullptr;
		setState(RobotHandState::EMPTY);
	}
}

void RobotHand::setupCollider()
{
	// OBox around sprite
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	((OBox*)m_collider)->setHalfExtents((Vector2)m_globalTransform[0] * 30, (Vector2)m_globalTransform[1] * 50);
	((OBox*)m_collider)->setCentre((Vector2)m_globalTransform[2]+(Vector2)m_globalTransform[1] * 95);
}
