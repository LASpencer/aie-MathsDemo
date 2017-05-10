#pragma once
#include "GameMode.h"
#include "SceneObject3D.h"
//HACK remove when I figure out how to do this myself
#include <glm/mat4x4.hpp>

//TODO this is just to test the idea works. Remove/refactor depending on choices

class Game3D :
	public GameMode
{
public:
	Game3D();
	~Game3D();

	virtual void startup();
	virtual void update(float deltaTime);
	virtual void draw(aie::Renderer2D*);

protected:
	float m_time;
	float m_cameraTurn; //HACK
	float m_pitch;
	float m_yaw;
	float m_roll;
	SceneObject3D m_sceneRoot;
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};

