#include "TankTurret.h"
#include "Texture.h"
#include "Input.h"


 
TankTurret::TankTurret() : m_turretSprite(new aie::Texture("./textures/barrelGreen.png"))
{
}


TankTurret::~TankTurret()
{
}

void TankTurret::update(float deltaTime)
{
	SceneObject::update(deltaTime);
}

void TankTurret::draw(aie::Renderer2D * renderer)
{
	// draw turret
	renderer->drawSpriteTransformed3x3(m_turretSprite, (float*)m_globalTransform, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f);
	SceneObject::draw(renderer);
}
