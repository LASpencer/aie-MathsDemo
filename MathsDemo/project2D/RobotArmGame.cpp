#include "RobotArmGame.h"
#include "RobotArm.h"
#include "Crate.h"
#include "Collider.h"

RobotArmGame::RobotArmGame()
{
}


RobotArmGame::~RobotArmGame()
{
}

void RobotArmGame::startup()
{
	if (!m_started) {
		// Place planes for game boundaries
		m_boundary[0] = Plane(Vector2(0, 1), 0);		// Bottom of screen
		m_boundary[1] = Plane(Vector2(1, 0), 0);		// Left of screen
		m_boundary[2] = Plane(Vector2(0, -1), 720);		// Top of screen
		m_boundary[3] = Plane(Vector2(-1, 0), 1280);	// Right of screen
		// Add RobotArm
		m_sceneRoot.addChild(new RobotArm({ 640, 360 }));
		// Place crates
		m_sceneRoot.addChild(new Crate({ 100, 100}));
		m_sceneRoot.addChild(new Crate({ 200,100 }, "./textures/car.png"));
		m_sceneRoot.addChild(new Crate({ 300,100 }, "./textures/ball.png"));
		m_sceneRoot.addChild(new Crate({ 200, 200 }));
		m_sceneRoot.addChild(new Crate({ 400,200 }, "./textures/car.png"));
		m_sceneRoot.addChild(new Crate({ 600,200 }, "./textures/ball.png"));
		m_sceneRoot.addChild(new Crate({ 300, 300 }));
		m_sceneRoot.addChild(new Crate({ 600,300 }, "./textures/car.png"));
		m_sceneRoot.addChild(new Crate({ 800,300 }, "./textures/ball.png"));
		m_sceneRoot.addChild(new Crate({ 400, 400 }));
		m_sceneRoot.addChild(new Crate({ 800,400 }, "./textures/car.png"));
		m_sceneRoot.addChild(new Crate({ 1200,400 }, "./textures/ball.png"));

		m_started = true;
	}
}

void RobotArmGame::update(float deltaTime)
{
	// Update scene objects
	m_sceneRoot.update(deltaTime);

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
				// Reverse direction of penetration vector for second object
				(*secondObject)->notifyCollision(*firstObject, -1 * collision.second);
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

void RobotArmGame::draw(aie::Renderer2D * renderer)
{
	renderer->setRenderColour(1, 1, 1, 1);
	m_sceneRoot.draw(renderer);
}
