#include "TankGame.h"
#include "Input.h"
#include "Tank.h"
#include "Wall.h"
#include "OBox.h"


TankGame::TankGame()
{
}


TankGame::~TankGame()
{
}

void TankGame::startup()
{
	if (!m_started) {
		m_sceneRoot.addChild(new Tank(Vector2(300, 200)));
		m_boundary[0] = Plane(Vector2(0, 1), 0);		// Bottom of screen
		m_boundary[1] = Plane(Vector2(1, 0), 0);		// Left of screen
		m_boundary[2] = Plane(Vector2(0, -1), 720);		// Top of screen
		m_boundary[3] = Plane(Vector2(-1, 0), 1280);	// Right of screen
		//Add walls
		m_sceneRoot.addChild(new Wall({ 100,100 }, { 600,100 }));
		m_sceneRoot.addChild(new Wall({ 100,600 }, { 200,300 }));
		m_sceneRoot.addChild(new Wall({ 1200,100 }, { 1200,400 }));
		m_started = true;
	}
}

void TankGame::update(float deltaTime)
{
	m_sceneRoot.update(deltaTime);
	// Perform any additions, removals, or transfers requested during update
	m_sceneRoot.updateChildList();

	std::vector<SceneObject*> objects = m_sceneRoot.getDescendants();
	//TODO check for collisions
	for (std::vector<SceneObject*>::iterator firstObject = objects.begin(); firstObject != objects.end(); ++firstObject) {
		//TODO figure out concrete type of firstObject
		for (std::vector<SceneObject*>::iterator secondObject = firstObject + 1; secondObject != objects.end(); ++secondObject) {
			//TODO check for collision
			//TODO if collision, figure out concrete type of second object and act accordingly
		}
	}
	//TODO destroy stopped/out of bounds bullets
}

void TankGame::draw(aie::Renderer2D* renderer)
{
	renderer->setRenderColour(1, 1, 1, 1);
	m_sceneRoot.draw(renderer);
}
