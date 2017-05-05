#pragma once
#include "Vector.h"

using namespace lasmath;

class Collider;

class Plane
{
public:
	Plane();
	Plane(Vector2 normal, float offset);
	~Plane();

	// Calculates distance from plane to point. Positive is in front, negative is behind
	float distanceToPlane(Vector2 point);
	

protected:
	Vector2 m_normal;
	float m_offset;
};

