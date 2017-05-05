#pragma once
#include "Vector.h"

using namespace lasmath;

class AABox;
class OBox;
class CircleCollider;
class Ray;

// Abstract class for collision testing objects
class Collider
{
public:
	Collider();
	~Collider();

	//TODO create collider bounding a set of points

	virtual bool doesCollide(Collider* other) = 0;
	virtual bool doesCollide(Vector2 point) = 0;

	// virtual functions to test collision with each collider subclass
	virtual bool doesCollideWithAABox(AABox* box) =0;
	virtual bool doesCollideWithOBox(OBox* box) = 0;
	virtual bool doesCollideWithCircle(CircleCollider* circle) = 0;

	// check if collider is hit by ray
	virtual bool isHitByRay(Ray* ray) = 0;
};

