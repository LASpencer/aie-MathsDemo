#pragma once
#include "SceneObject.h"
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

protected:
	aie::Texture* m_sprite;
};

