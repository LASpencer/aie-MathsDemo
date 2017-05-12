#pragma once
#include "GameMode.h"
#include "SceneObject.h"
#include "Plane.h"

// A game where a Tank drives around shooting bullets at obstacles and walls
class TankGame :
	public GameMode
{
public:
	TankGame();
	~TankGame();

	virtual void startup();
	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D* renderer);

protected:
	SceneObject m_sceneRoot;
	Plane		m_boundary[4];

};

