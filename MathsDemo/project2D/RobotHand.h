#pragma once
#include "SceneObject.h"

// States RobotHand can be in
enum class RobotHandState {	EMPTY,			// Nothing held in hand
							GRABBING,		// Hand trying to grab a crate
							HOLDING,		// Crate held in hand
							RELEASING};		// Hand trying to release crate

class Crate;

// A SceneObject that can pick up and drop Crates
class RobotHand :
	public SceneObject
{
public:
	RobotHand();
	RobotHand(aie::Texture* sprite);
	~RobotHand();

	void setSprite(aie::Texture* sprite) {
		m_sprite = sprite;
	}

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	RobotHandState getState() {
		return m_state;
	}
	
	void setState(RobotHandState state);

	virtual void notifyCollision(SceneObject* other, Vector2 penetration);
	

protected:
	aie::Texture* m_sprite;
	RobotHandState m_state;				//Current state of hand
	Crate*			m_heldCrate;		//Pointer to crate held

	// Reparent crate to scene graph's root
	void releaseCrate();

	// Create an OBox around the sprite
	virtual void setupCollider();
};

