#pragma once
#include "SceneObject.h"
class Wall :
	public SceneObject
{
public:
	Wall();
	~Wall();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	virtual void notifyCollision(SceneObject* other);

protected:
	virtual void setupCollider();
};

