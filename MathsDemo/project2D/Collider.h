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

	//TODO returns pair of bool, minimum axis of overlap
	virtual std::pair<bool, Vector2> doesCollide(Collider* other) = 0;
	virtual std::pair<bool, Vector2> doesCollide(Vector2 point) = 0;

	// virtual functions to test collision with each collider subclass
	//TODO returns pair of bool, minimum axis of overlap
	virtual std::pair<bool, Vector2> doesCollideWithAABox(AABox* box) =0;
	virtual std::pair<bool, Vector2> doesCollideWithOBox(OBox* box) = 0;
	virtual std::pair<bool, Vector2> doesCollideWithCircle(CircleCollider* circle) = 0;

	// check if collider is hit by ray
	virtual bool isHitByRay(Ray* ray) = 0;

	// TODO collision with plane returning depth of penetration
};

