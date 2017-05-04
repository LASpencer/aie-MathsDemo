#include "TankTurret.h"
#include "Texture.h"
#include "Input.h"

const int TankTurret::LEFT_AIM_KEY = aie::INPUT_KEY_LEFT;
const int TankTurret::RIGHT_AIM_KEY = aie::INPUT_KEY_RIGHT;
const float TankTurret::AIM_RATE = 1.5f;
 
TankTurret::TankTurret() : m_turretSprite(new aie::Texture("./textures/barrelGreen.png"))
{
}


TankTurret::~TankTurret()
{
}

void TankTurret::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	bool leftTurn = input->isKeyDown(LEFT_AIM_KEY);
	bool rightTurn = input->isKeyDown(RIGHT_AIM_KEY);
	// TODO limited firing arc
	if (leftTurn && !rightTurn) {
		rotate(AIM_RATE*deltaTime);
	} else if (rightTurn && !leftTurn) {
		rotate(-AIM_RATE*deltaTime);
	}

	SceneObject::update(deltaTime);
}

void TankTurret::draw(aie::Renderer2D * renderer)
{
	// draw turret
	renderer->drawSpriteTransformed3x3(m_turretSprite, (float*)m_globalTransform, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f);
	SceneObject::draw(renderer);
}
