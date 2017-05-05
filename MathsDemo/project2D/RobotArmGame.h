#pragma once
#include "GameMode.h"
#include "SceneObject.h"

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
	SceneObject m_sceneRoot;
};

