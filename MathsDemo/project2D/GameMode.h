#pragma once
#include "Renderer2D.h"

// Abstract class for games that can be run by Application2D
class GameMode
{
public:
	GameMode();
	~GameMode();

	// Setup all objects and variable required for the game
	virtual void startup() = 0;
	// Update the game state
	virtual void update(float deltaTime) = 0;
	// Draw game with renderer
	virtual void draw(aie::Renderer2D* renderer) = 0;

	// Check if game has been started up
	bool isStarted() {
		return m_started;
	}

protected:
	bool m_started;		//flag for whether the game has been started up
};

