#include "Plane.h"



Plane::Plane() : m_normal({ 1,0 }), m_offset(0.0f)
{
}

Plane::Plane(Vector2 normal, float offset) : m_offset(offset)
{
	bool valid = normal.normalise();
	if (!valid) {
		//TODO throw argument exception
	}
	m_normal = normal;
}


Plane::~Plane()
{
}

float Plane::distanceToPlane(Vector2 point)
{
	return m_normal.dot(point) + m_offset;
}
