#pragma once
#include "SceneObject.h"
class Obstacle :
	public SceneObject
{
public:
	Obstacle();
	~Obstacle();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	virtual void notifyCollision(SceneObject* other);

protected:
	virtual void setupCollider();
};

