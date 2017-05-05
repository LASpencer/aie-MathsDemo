#include "Game3D.h"

#include "Gizmos.h"
#include "Input.h"
//HACK can I reimplement this myself?
//TODO GizmoAdapter class, so I can use my maths library to control gizmos
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Application2D.h"

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
		m_started = true;
	}
}

void Game3D::update(float deltaTime)
{
	// query time since application started
	m_time += deltaTime;

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(m_time) * 10, 10, glm::cos(m_time) * 10),
							   vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// demonstrate a few shapes
	Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	mat4 t = glm::rotate(m_time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);
	Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();
}

void Game3D::draw(aie::Renderer2D *)
{
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}
