#pragma once
#include "SceneObject.h"

enum class RobotHandState {EMPTY, GRABBING, HOLDING, RELEASING};

class Crate;

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
	RobotHandState m_state;
	Crate*			m_heldCrate;

	void releaseCrate();
	virtual void setupCollider();
};

