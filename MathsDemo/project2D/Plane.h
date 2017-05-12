#pragma once
#include "Vector.h"

using namespace lasmath;

class Collider;

// A line dividing a 2D plane into points in front of and behind it
class Plane
{
public:
	Plane();
	Plane(Vector2 normal, float offset);
	~Plane();

	// Calculates distance from plane to point. Positive is in front, negative is behind
	float distanceToPlane(Vector2 point);
	
	Vector2 getNormal() {
		return m_normal;
	}
	float getOffset() {
		return m_offset;
	}

protected:
	Vector2 m_normal;
	float m_offset;		//Distance from plane to origin
};

