#include "Obstacle.h"



Obstacle::Obstacle()
{
}


Obstacle::~Obstacle()
{
}

void Obstacle::update(float deltaTime)
{
	//TODO
	SceneObject::update(deltaTime);
}

void Obstacle::draw(aie::Renderer2D * renderer)
{
	//TODO
	SceneObject::draw(renderer);
}

void Obstacle::notifyCollision(SceneObject * other)
{
	//TODO
}

void Obstacle::setupCollider()
{
	//TODO
}
