#include "OBox.h"
#include "AABox.h"
#include "Ray.h"


OBox::OBox() : m_xExtent({1,0}), m_yExtent({0,1}), m_centre()
{
}

OBox::OBox(Vector2 xExtent, Vector2 yExtent, Vector2 centre) : m_xExtent(xExtent), m_yExtent(yExtent), m_centre(centre)
{
	//TODO throw exception if invalid vectors
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
	// If any corner of this is in the box, they collide
	if (box->doesCollide(m_centre + m_xExtent + m_yExtent) ||
		box->doesCollide(m_centre + m_xExtent - m_yExtent) ||
		box->doesCollide(m_centre - m_xExtent + m_yExtent) ||
		box->doesCollide(m_centre - m_xExtent - m_yExtent)) {
		return true;
	}	// box may be entirely within this, so check one of its corners
	else if (doesCollide(box->getMinCorner())) {
		return true;
	}
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

bool OBox::isHitByRay(Ray * ray)
{
	return ray->doesCollide(this);
}

Matrix3 OBox::getBoxMatrix()
{
	Matrix3 BoxMatrix;
	BoxMatrix[0] = (Vector3)m_xExtent;
	BoxMatrix[1] = (Vector3)m_yExtent;
	BoxMatrix[2] = (Vector3)m_centre;
	BoxMatrix[2][2] = 1.0f;				// To make it a homogeneous matrix
	return BoxMatrix;
}

Matrix3 OBox::getInverseTransform()
{
	//TODO possible optimization: memoize result, recalculate if obox changes (set dirty flag)
	Matrix3 inverse;
	getBoxMatrix().calculateInverse(inverse);
	return inverse;
}
