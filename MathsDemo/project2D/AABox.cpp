#include "AABox.h"



AABox::AABox()
{
}


AABox::~AABox()
{
}

bool AABox::doesCollide(Collider * other)
{
	// Calls method on other collider to test collision with AABox
	return other->doesCollideWithAABox(this);
}

bool AABox::doesCollide(Vector2 point)
{
	if (point[0]<m_min[0] || point[0]>m_max[0] || point[1]<m_min[1] || point[1]>m_max[1]) {
		return false;
	}
	else {
		return true;
	}
}
