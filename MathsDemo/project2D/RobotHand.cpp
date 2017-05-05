#include "RobotHand.h"
#include "Texture.h"


RobotHand::RobotHand() : m_sprite(nullptr)
{
}

RobotHand::RobotHand(aie::Texture * sprite) : m_sprite(sprite)
{
}


RobotHand::~RobotHand()
{
	delete m_sprite;
}

void RobotHand::update(float deltaTime)
{
	SceneObject::update(deltaTime);
}

void RobotHand::draw(aie::Renderer2D * renderer)
{
	if (m_sprite != nullptr) {
		renderer->drawSpriteTransformed3x3(m_sprite, (float*)m_globalTransform, 0.0f, 0.0f, 0.0f, 0.5f, 0.05f);
	}
	SceneObject::draw(renderer);
}
