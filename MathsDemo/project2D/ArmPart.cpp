#include "ArmPart.h"
#include "Texture.h"


ArmPart::ArmPart() : m_sprite(nullptr)
{
}

ArmPart::ArmPart(aie::Texture * sprite) : m_sprite(sprite)
{
}


ArmPart::~ArmPart()
{
	delete m_sprite;
}

void ArmPart::update(float deltaTime)
{
	SceneObject::update(deltaTime);
}

void ArmPart::draw(aie::Renderer2D * renderer)
{
	if (m_sprite != nullptr) {
		// Draw sprite with origin 7% of the way along the arm
		renderer->drawSpriteTransformed3x3(m_sprite, (float*)m_globalTransform, 0.0f, 0.0f, 0.0f, 0.5f, 0.07f);
	}
	SceneObject::draw(renderer);
}
