#include "OBox.h"



OBox::OBox()
{
}

OBox::OBox(Vector2 xExtent, Vector2 yExtent, Vector2 centre) : m_xExtent(xExtent), m_yExtent(yExtent), m_centre(centre)
{
}


OBox::~OBox()
{
}

bool OBox::doesCollide(Collider * other)
{
	return other->doesCollideWithOBox(this);
}

bool OBox::doesCollide(Vector2 point)
{
	bool collision = true;
	Vector2 displacement = m_centre - point;
	// If dot product of displacement and extent is greater than the extent's magnitude, point lies outside the box
	if (m_xExtent.compareMagnitude(m_xExtent.dot(displacement)) == -1) {
		collision = false;
	} else if (m_yExtent.compareMagnitude(m_yExtent.dot(displacement)) == -1){
		collision = false;
	}
	return collision;
}

bool OBox::doesCollideWithAABox(AABox * box)
{
	//TODO
	return false;
}

bool OBox::doesCollideWithOBox(OBox * box)
{
	//TODO
	return false;
}

bool OBox::doesCollideWithCircle(CircleCollider * circle)
{
	//TODO
	return false;
}
