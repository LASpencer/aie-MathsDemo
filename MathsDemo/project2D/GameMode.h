#pragma once
#include "Renderer2D.h"

class GameMode
{
public:
	GameMode();
	~GameMode();

	virtual void startup() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(aie::Renderer2D* renderer) = 0;

	bool isStarted() {
		return m_started;
	}

protected:
	bool m_started;
};

