#pragma once
#include "Vector.h"

using namespace lasmath;

class Collider;
class CircleCollider;
class AABox;
class OBox;

// A ray used for collision testing
class Ray
{
public:
	Ray();
	Ray(Vector2 origin, Vector2 direction);
	~Ray();

	//accessors and mutators
	Vector2 getOrigin() {
		return m_origin;
	}
	Vector2 getDirection() {
		return m_direction;
	}

	void setOrigin(Vector2 origin);

	void setDirection(Vector2 direction);

	// Returns the closest point on the ray to given point
	Vector2 findClosestPoint(Vector2 Point);
	
	// test collision with collider of unknown type
	bool doesCollide(Collider* collider);
	// Test collision with circle
	bool doesCollide(CircleCollider* circle);
	// Test collision with Axis Aligned Bounding Box
	bool doesCollide(AABox* box);
	// Test collision with Oriented Bounding Box
	bool doesCollide(OBox* box);

protected:
	Vector2 m_origin;
	Vector2 m_direction;
};

