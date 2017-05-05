#include "TankGame.h"
#include "Input.h"
#include "Tank.h"


TankGame::TankGame()
{
}


TankGame::~TankGame()
{
}

void TankGame::startup()
{
	if (!m_started) {
		m_sceneRoot.addChild(new Tank(Vector2(300, 200)));
		m_started = true;
	}
}

void TankGame::update(float deltaTime)
{
	m_sceneRoot.update(deltaTime);
}

void TankGame::draw(aie::Renderer2D* renderer)
{
	renderer->setRenderColour(1, 1, 1, 1);
	m_sceneRoot.draw(renderer);
}
