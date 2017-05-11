#include "Game3D.h"
#include "Application2D.h"
#include "GLMAdaptor.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "Rocket.h"
#include "Camera.h"

using aie::Gizmos;

Game3D::Game3D()
{
}


Game3D::~Game3D()
{
	Gizmos::destroy();
}

void Game3D::startup()
{
	if (!m_started) {
		//TODO startup
		// initialise gizmo primitive counts
		Gizmos::create(10000, 10000, 10000, 10000);

		//Rotate root so Z axis is up
		Matrix4 flipAxes;
		flipAxes.setEulerRotate(0.0f, -1.57077f, 3.141592f);
		m_sceneRoot.setLocalTransform(flipAxes);
		// add planets to root
		m_sceneRoot.addChild(new Planet(1, 4, 0.5f, 0, 0.5f, 0, 0.1f));
		Planet* planet1 = new Planet(1, 7, 0.3f, 1.5f, 0.3f, 1, 0);
		planet1->addChild(new Planet(0.2f, 2, 2, 0, 0, 0, 0, {0.56f,0.56f,0.6f,0.5f}));
		m_sceneRoot.addChild(planet1);
		//TODO more planets
		//TODO camera follows a planet
		// add rocket
		m_rocket = new Rocket();
		m_sceneRoot.addChild(m_rocket);
		// put camera behind rocket and looking slightly down
		m_rocketCamera = new Camera(3, -3.1415926f*0.5f,-1);
		m_rocket->addChild(m_rocketCamera);
		m_worldCamera = new Camera(20, 3.1415926f*0.25f, -3.1415926f*0.25f);
		m_sceneRoot.addChild(m_worldCamera);
		m_camera = m_worldCamera;

		// create camera transforms
		m_viewMatrix = m_camera->lookAt();
		m_projectionMatrix = GLMAdaptor::perspective(3.14159265f * 0.25f,
													1280.0f / 720.0f,		//HACK until I write a way to get it from the application2d object
													0.1f, 1000.f);
		m_started = true;
	}
}

void Game3D::update(float deltaTime)
{
	// get input
	aie::Input* input = aie::Input::getInstance();
	// Move selected camera
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT)) {
		//pan right
		m_camera->pan(deltaTime*Camera::PAN_RATE);
	}
	else if (input->isKeyDown(aie::INPUT_KEY_LEFT)) {
		//pan left
		m_camera->pan(-deltaTime*Camera::PAN_RATE);
	}
	if (input->isKeyDown(aie::INPUT_KEY_UP)) {
		//tilt forward
		m_camera->tilt(deltaTime*Camera::TILT_RATE);
	}
	else if (input->isKeyDown(aie::INPUT_KEY_DOWN)) {
		//tilt back
		m_camera->tilt(-deltaTime*Camera::TILT_RATE);
	}
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT_SHIFT)) {
		//zoom in
		m_camera->zoom(-deltaTime*Camera::ZOOM_RATE);
	}
	else if (input->isKeyDown(aie::INPUT_KEY_RIGHT_CONTROL)) {
		//zoom out
		m_camera->zoom(deltaTime*Camera::ZOOM_RATE);
	}
	//switch cameras
	if (input->wasKeyPressed(aie::INPUT_KEY_V)) {
		if (m_camera == m_worldCamera) {
			m_camera = m_rocketCamera;
		}
		else {
			m_camera = m_worldCamera;
		}
	}


	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 grey(0.2f, 0.2f, 0.2f, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : grey);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : grey);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// Update scene objects
	m_sceneRoot.update(deltaTime);
	m_sceneRoot.updateChildList();

	// Get new view matrix
	m_viewMatrix = m_camera->lookAt();
}

void Game3D::draw(aie::Renderer2D *)
{
	Gizmos::draw(GLMAdaptor::Matrix4Converter(m_projectionMatrix * m_viewMatrix));
}
