#include "Wall.h"



Wall::Wall()
{
}


Wall::~Wall()
{
}

void Wall::update(float deltaTime)
{
	//TODO
	SceneObject::update(deltaTime);
}

void Wall::draw(aie::Renderer2D * renderer)
{
	//TODO
	SceneObject::draw(renderer);
}

void Wall::notifyCollision(SceneObject * other)
{
	//TODO
}

void Wall::setupCollider()
{
	//TODO
	SceneObject::setupCollider();
}
