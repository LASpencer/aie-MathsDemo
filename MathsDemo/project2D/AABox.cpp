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

bool AABox::doesCollideWithAABox(AABox * box)
{
	if (box->m_min[0] > m_max[0] || box->m_min[1] > m_max[1] || box->m_max[0] < m_min[0] || box->m_max[1] < m_min[1]) {
		return false;
	}
	else {
		return true;
	}
}

bool AABox::doesCollideWithOBox(OBox * box)
{
	//TODO when OBox written, write collision code either here or in OBox
	return false;
}

bool AABox::doesCollideWithCircle(CircleCollider * circle)
{
	//TODO when CircleCollider written, write collision code here or in circle
	return false;
}
