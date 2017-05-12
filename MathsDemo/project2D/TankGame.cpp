#include "TankGame.h"
#include "Input.h"
#include "Tank.h"
#include "Obstacle.h"
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
		
		// Place planes for game boundaries
		m_boundary[0] = Plane(Vector2(0, 1), 0);		// Bottom of screen
		m_boundary[1] = Plane(Vector2(1, 0), 0);		// Left of screen
		m_boundary[2] = Plane(Vector2(0, -1), 720);		// Top of screen
		m_boundary[3] = Plane(Vector2(-1, 0), 1280);	// Right of screen

		// Add tank
		m_sceneRoot.addChild(new Tank(Vector2(300, 200)));
		//Add walls
		m_sceneRoot.addChild(new Wall({ 100,100 }, { 600,100 }));
		m_sceneRoot.addChild(new Wall({ 100,600 }, { 200,300 }));
		m_sceneRoot.addChild(new Wall({ 1200,100 }, { 1200,400 }));
		m_sceneRoot.addChild(new Wall({ 450,450 }, { 550,450 }));
		m_sceneRoot.addChild(new Wall({ 450,450 }, { 450,550 }));
		m_sceneRoot.addChild(new Wall({ 550,450 }, { 550,550 }));
		m_sceneRoot.addChild(new Wall({ 450,600 }, { 550,600 }));
		m_sceneRoot.addChild(new Wall({ 450,550}, { 480,550 }));
		m_sceneRoot.addChild(new Wall({ 520,550 }, { 550,550 }));
		//Add obstacles
		m_sceneRoot.addChild(new Obstacle({200,200}, "./textures/ship.png",2));
		m_sceneRoot.addChild(new Obstacle({ 500,500 }, "./textures/ship.png", 2));
		m_sceneRoot.addChild(new Obstacle({ 800,500 }, "./textures/ArmShoulder.png", 2));
		m_sceneRoot.addChild(new Obstacle({ 600,300 }, "./textures/car.png"));
		m_sceneRoot.addChild(new Obstacle({ 1000,200 }, "./textures/grass.png"));
		m_sceneRoot.addChild(new Obstacle({400,500}));
		m_sceneRoot.addChild(new Obstacle({ 800,600 }, "./textures/tankBeige.png", 3));

		m_started = true;
	}
}

void TankGame::update(float deltaTime)
{
	// Update scene objects
	m_sceneRoot.update(deltaTime);
	// Perform any additions, removals, or transfers requested during update
	m_sceneRoot.updateChildList();

	// Get vector of all scene objects
	std::vector<SceneObject*> objects = m_sceneRoot.getDescendants();
	// check for collision between each pair of scene objects
	for (std::vector<SceneObject*>::iterator firstObject = objects.begin(); firstObject != objects.end(); ++firstObject) {
		for (std::vector<SceneObject*>::iterator secondObject = firstObject + 1; secondObject != objects.end(); ++secondObject) {
			// Test collision between object's colliders
			std::pair<bool, Vector2> collision = (*firstObject)->getCollider()->doesCollide((*secondObject)->getCollider());
			// If they collide, pass penetration vector to both objects
			if (collision.first) {
				(*firstObject)->notifyCollision(*secondObject, collision.second);
				// Reverse direction of penetration vector for second obj
				(*secondObject)->notifyCollision(*firstObject, -1*collision.second);
			}
		}
		// Also check if object is out of bounds
		for (size_t i = 0; i < 4; ++i) {
			// Test collision between object and boundary
			std::pair<bool, Vector2> boundaryCollision = (*firstObject)->getCollider()->doesCollide(m_boundary[i]);
			if (boundaryCollision.first) {
				// If they collide, pass penetration vector to object
				(*firstObject)->notifyOutOfBounds(boundaryCollision.second);
			}
		}
	}
}

void TankGame::draw(aie::Renderer2D* renderer)
{
	renderer->setRenderColour(1, 1, 1, 1);
	m_sceneRoot.draw(renderer);
}
