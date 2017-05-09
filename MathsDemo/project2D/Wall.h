#pragma once
#include "SceneObject.h"
class Wall :
	public SceneObject
{
public:
	Wall();
	Wall(Vector2 start, Vector2 end);
	~Wall();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	virtual void notifyCollision(SceneObject* other);

	static const float WIDTH;
protected:
	Vector2 m_start, m_end;
	virtual void setupCollider();
};

