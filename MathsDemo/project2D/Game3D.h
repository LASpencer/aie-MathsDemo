#pragma once
#include "GameMode.h"
#include "SceneObject3D.h"
#include "Matrix.h"

class Rocket;
class Camera;

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
	SceneObject3D m_sceneRoot;
	Rocket*		m_rocket;
	Camera*		m_camera;
	//TODO put cameras in array, iterate over it when v pressed
	Camera*		m_rocketCamera;
	Camera*		m_worldCamera;
	Matrix4		m_viewMatrix;
	Matrix4		m_projectionMatrix;
};

