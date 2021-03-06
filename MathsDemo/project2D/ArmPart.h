#pragma once
#include "SceneObject.h"

// A section of the robot arm which can be rotated around its joint
class ArmPart :
	public SceneObject
{
public:
	ArmPart();
	ArmPart(aie::Texture* sprite);
	~ArmPart();

	void setSprite(aie::Texture* sprite) {
		m_sprite = sprite;
	}

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

protected:
	aie::Texture* m_sprite;
};

