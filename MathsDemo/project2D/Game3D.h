#pragma once
#include "GameMode.h"
#include "SceneObject3D.h"
#include "Matrix.h"

class Rocket;
class Camera;

// A 3D game demonstrating transformations with Matrix4
class Game3D :
	public GameMode
{
public:
	Game3D();
	~Game3D();

	virtual void startup();
	virtual void update(float deltaTime);
	// Render objects to screen with Gizmos
	virtual void draw(aie::Renderer2D* renderer);

protected:
	SceneObject3D m_sceneRoot;
	Rocket*		m_rocket;
	std::vector<Camera*>::iterator	m_camera;
	std::vector<Camera*> m_cameraArray;
	Matrix4		m_viewMatrix;
	Matrix4		m_projectionMatrix;
};

