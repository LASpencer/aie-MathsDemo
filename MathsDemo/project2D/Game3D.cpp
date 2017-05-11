#include "Game3D.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Application2D.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "Rocket.h"


using glm::vec3;
using glm::vec4;
using glm::mat4;
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

		// create simple camera transforms
		m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
		m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
			1280.0f/ 720.0f,		//HACK until I write a way to get it from the application2d object
			0.1f, 1000.f);
		m_time = 0.0f;

		m_cameraTurn = 0.0f;
		m_pitch = 0.0f;
		m_yaw = 0.0f;
		m_roll = 0.0f;

		//Rotate root so Z axis is up
		Matrix4 flipAxes;
		flipAxes.setEulerRotate(0.0f, -1.57077f, 3.141592f);
		m_sceneRoot.setLocalTransform(flipAxes);
		// add planets to root
		m_sceneRoot.addChild(new Planet(1, 4, 0.5f, 0, 0.5f, 0, 0.1f));
		Planet* planet1 = new Planet(1, 7, 0.3f, 1.5f, 0.3f, 1, 0);
		planet1->addChild(new Planet(0.2f, 2, 2, 0, 0, 0, 0, {0.56f,0.56f,0.6f,0.5f}));
		m_sceneRoot.addChild(planet1);
		// add rocket
		m_sceneRoot.addChild(new Rocket());
		m_started = true;
	}
}

void Game3D::update(float deltaTime)
{
	// set time since application started
	m_time += deltaTime;

	// get input
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT)) {
		m_cameraTurn +=  deltaTime;
	}
	else if (input->isKeyDown(aie::INPUT_KEY_LEFT)) {
		m_cameraTurn -=  deltaTime;
	}

	//TODO camera follows rocket?
	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::cos(m_cameraTurn) * -10, 10, glm::sin(m_cameraTurn) * 10),
							   vec3(0), vec3(0, 1, 0));

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

	// demonstrate a few shapes
	//Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	//
	//Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	//Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	//Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	//Matrix4 rotate, convert, faceX;
	//// HACK axes are different to expected:
	//convert.setEulerRotate(0.0f, -1.57077f, 3.141592f);
	//faceX.setRotateZ(-1.57077f);
	////rotate.setTaitBryanRotate(m_roll, m_yaw, m_pitch);
	//rotate.setTaitBryanRotate(m_yaw, m_pitch, m_roll);
	//rotate = convert * rotate * faceX;
	//rotate[3] = { -2,0,0,1 };
	//
	////HACK use adaptor class to convert
	//// TODO make lasmath to glm adaptor
	//mat4 t = mat4(rotate[0][0], rotate[0][1], rotate[0][2], rotate[0][3],
	//	rotate[1][0], rotate[1][1], rotate[1][2], rotate[1][3],
	//	rotate[2][0], rotate[2][1], rotate[2][2], rotate[2][3],
	//	rotate[3][0], rotate[3][1], rotate[3][2], rotate[3][3]);
	//mat4 t = glm::rotate(m_time, glm::normalize(vec3(1, 1, 1)));
	//t[3] = vec4(-2, 0, 0, 1);

	//Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	//rotate.setEulerRotate(m_pitch, m_yaw, m_roll);
	////HACK use adaptor class to convert
	//// TODO make lasmath to glm adaptor
	//t = mat4(rotate[0][0], rotate[0][1], rotate[0][2], rotate[0][3],
	//	rotate[1][0], rotate[1][1], rotate[1][2], rotate[1][3],
	//	rotate[2][0], rotate[2][1], rotate[2][2], rotate[2][3],
	//	rotate[3][0], rotate[3][1], rotate[3][2], rotate[3][3]);
	//Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f), &t);

	// Update scene root
	m_sceneRoot.update(deltaTime);
	m_sceneRoot.updateChildList();
}

void Game3D::draw(aie::Renderer2D *)
{
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}
