#pragma once
#include "Vector.h"

using namespace lasmath;

class AABox;
class OBox;
class CircleCollider;
class Ray;
class Plane;

// Abstract class for collision testing objects
class Collider
{
public:
	Collider();
	~Collider();

	// Test collision with collider of unknown concrete type
	virtual std::pair<bool, Vector2> doesCollide(Collider* other) = 0;

	// Test collision with point
	virtual std::pair<bool, Vector2> doesCollide(Vector2 point) = 0;

	// Tests collision with plane
	virtual std::pair<bool, Vector2> doesCollide(Plane plane) = 0;

	// virtual functions to test collision with each collider subclass
	
	// Test collision with AABox
	virtual std::pair<bool, Vector2> doesCollideWithAABox(AABox* box) =0;
	// Test collision with OBox
	virtual std::pair<bool, Vector2> doesCollideWithOBox(OBox* box) = 0;
	// Test collision with CircleCollider
	virtual std::pair<bool, Vector2> doesCollideWithCircle(CircleCollider* circle) = 0;

	// check if collider is hit by ray
	virtual bool isHitByRay(Ray* ray) = 0;
};

