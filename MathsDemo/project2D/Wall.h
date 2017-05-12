#pragma once
#include "SceneObject.h"

// A wall which stops movement and reflects bullets
class Wall :
	public SceneObject
{
public:
	Wall();
	Wall(Vector2 start, Vector2 end);
	~Wall();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	static const float WIDTH;		//Width of wall in pixels
protected:
	Vector2 m_start, m_end;			//Start and End points of the wall

	// Create an OBox around the wall
	virtual void setupCollider();
};

