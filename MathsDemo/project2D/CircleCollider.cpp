#include "CircleCollider.h"



CircleCollider::CircleCollider()
{
}


CircleCollider::~CircleCollider()
{
}

bool CircleCollider::doesCollide(Collider * other)
{
	return other->doesCollideWithCircle(this);
}

bool CircleCollider::doesCollide(Vector2 point)
{
	Vector2 displacement = point - m_centre;
	if (displacement.magnitudeSquared() > (m_radius*m_radius)) {
		return false;
	}
	else {
		return true;
	}
}

bool CircleCollider::doesCollideWithAABox(AABox * box)
{
	return false;
}

bool CircleCollider::doesCollideWithOBox(OBox * box)
{
	return false;
}

bool CircleCollider::doesCollideWithCircle(CircleCollider * circle)
{
	return false;
}
