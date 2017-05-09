#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	m_font = new aie::Font("./font/consolas.ttf", 32);
	m_tankGame = new TankGame();
	m_armGame = new RobotArmGame();
	m_3dGame = new Game3D();

	// Start with TankGame
	m_game = m_tankGame;
	m_game->startup();

	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	return true;
}

void Application2D::shutdown() {
	
	delete m_3dGame;
	delete m_armGame;
	delete m_tankGame;
	delete m_font;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// check for changing game mode
	if (input->isKeyDown(aie::INPUT_KEY_1)) {
		m_game = m_tankGame;
		m_game->startup();
	}
	else if (input->isKeyDown(aie::INPUT_KEY_2)) {
		m_game = m_armGame;
		m_game->startup();
	}
	else if (input->isKeyDown(aie::INPUT_KEY_3)) {
		m_game = m_3dGame;
		m_game->startup();
	}
	// reset games
	if (input->isKeyDown(aie::INPUT_KEY_BACKSPACE)) {
		//delete games
		delete m_tankGame;
		delete m_armGame;
		delete m_3dGame;
		//create new games
		m_tankGame = new TankGame();
		m_armGame = new RobotArmGame();
		m_3dGame = new Game3D();
		// startup tank game
		m_game = m_tankGame;
		m_game->startup();
	}

	m_game->update(deltaTime);
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	m_game->draw(m_2dRenderer);
	/*m_2dRenderer->setRenderColour(1, 1, 1, 1);
	m_sceneRoot.draw(m_2dRenderer);*/
	
	// output some text, uses the last used colour
	m_2dRenderer->setRenderColour(1, 1, 0, 1);
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	// done drawing sprites
	m_2dRenderer->end();
}