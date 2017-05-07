#include "RobotArmGame.h"
#include "RobotArm.h"


RobotArmGame::RobotArmGame()
{
}


RobotArmGame::~RobotArmGame()
{
}

void RobotArmGame::startup()
{
	if (!m_started) {
		//TODO setup game
		m_sceneRoot.addChild(new RobotArm({640, 360}));
		m_started = true;
	}
}

void RobotArmGame::update(float deltaTime)
{
	m_sceneRoot.update(deltaTime);
	// Perform any additions, removals, or transfers requested during update
	m_sceneRoot.updateChildList();
}

void RobotArmGame::draw(aie::Renderer2D * renderer)
{
	m_sceneRoot.draw(renderer);
}
