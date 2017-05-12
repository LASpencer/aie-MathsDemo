#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Audio.h"
#include "SceneObject.h"
#include "GameMode.h"
#include "TankGame.h"
#include "RobotArmGame.h"
#include "Game3D.h"

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	GameMode*			m_game;
	TankGame*			m_tankGame;
	RobotArmGame*		m_armGame;
	Game3D*				m_3dGame;

	float m_cameraX, m_cameraY;
};