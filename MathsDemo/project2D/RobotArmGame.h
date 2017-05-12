#pragma once
#include "GameMode.h"
#include "SceneObject.h"
#include "Plane.h"

// A game where a RobotArm picks up and moves around Crates
class RobotArmGame :
	public GameMode
{
public:
	RobotArmGame();
	~RobotArmGame();

	virtual void startup();
	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D* renderer);

protected:
	SceneObject			m_sceneRoot;
	Plane				m_boundary[4];
};

