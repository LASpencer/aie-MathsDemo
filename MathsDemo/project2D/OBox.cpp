#include "OBox.h"
#include "AABox.h"
#include "CircleCollider.h"
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
	//TODO tests
	// get each corner of the boxes
	Vector2 thisCorners[4];
	Vector2 otherCorners[4];
	thisCorners[0] = m_centre + m_xExtent + m_yExtent;
	thisCorners[1] = m_centre + m_xExtent - m_yExtent;
	thisCorners[2] = m_centre - m_xExtent - m_yExtent;
	thisCorners[3] = m_centre - m_xExtent + m_yExtent;

	otherCorners[0] = box->m_centre + box->m_xExtent + box->m_yExtent;
	otherCorners[1] = box->m_centre + box->m_xExtent - box->m_yExtent;
	otherCorners[2] = box->m_centre - box->m_xExtent - box->m_yExtent;
	otherCorners[3] = box->m_centre - box->m_xExtent + box->m_yExtent;

	// get each axis to test
	Vector2 axis[8];
	for (size_t i = 0; i < 4; i++) {
		Vector2 thisSide, otherSide;
		if(i == 3){
			// Side from last corner to first
			thisSide = thisCorners[0] - thisCorners[3];
			otherSide = otherCorners[0] - otherCorners[3];
		}
		else {
			// Side from current corner to next
			thisSide = thisCorners[i + 1] - thisCorners[i];
			otherSide = otherCorners[i + 1] - otherCorners[i];
		}
		// Axis is normal of side
		axis[i][0] = -(thisSide[1]);
		axis[i][1] = thisSide[0];
		axis[i + 4][0] = -(otherSide[1]);
		axis[i + 4][1] = otherSide[0];
	}
	// Project boxes onto each axis and check for overlap
	for (size_t i = 0; i < 8; i++) {
		float thisMin, otherMin = INFINITY;
		float thisMax, otherMax = -INFINITY;
		for (size_t corner = 0; corner < 4; corner++) {
			float thisProjection = axis[i].dot(thisCorners[corner]);
			float otherProjection = axis[i].dot(otherCorners[corner]);
			thisMin = std::min(thisMin, thisProjection);
			thisMax = std::max(thisMax, thisProjection);
			otherMin = std::min(otherMin, otherProjection);
			otherMax = std::max(otherMax, otherProjection);
		}
		if (otherMin > thisMax || thisMin > otherMax) {
			return false;
		}
	}
	return true;
}

bool OBox::doesCollideWithCircle(CircleCollider * circle)
{
	//TODO test
	Vector2 thisCorners[4];
	thisCorners[0] = m_centre + m_xExtent + m_yExtent;
	thisCorners[1] = m_centre + m_xExtent - m_yExtent;
	thisCorners[2] = m_centre - m_xExtent - m_yExtent;
	thisCorners[3] = m_centre - m_xExtent + m_yExtent;
	// get each axis to test
	Vector2 axis[4];
	for (size_t i = 0; i < 4; i++) {
		Vector2 thisSide;
		if (i == 3) {
			// Side from last corner to first
			thisSide = thisCorners[0] - thisCorners[3];
		}
		else {
			// Side from current corner to next
			thisSide = thisCorners[i + 1] - thisCorners[i];
		}
		// Axis is normal of side
		axis[i][0] = -(thisSide[1]);
		axis[i][1] = thisSide[0];
	}
	// Project boxe and circle onto each axis and check for overlap
	for (size_t i = 0; i < 8; i++) {
		float thisMin= INFINITY;
		float thisMax= -INFINITY;
		float circleCentre = axis[i].dot(circle->getCentre());
		float circleMin = circleCentre - circle->getRadius();
		float circleMax = circleCentre + circle->getRadius();
		for (size_t corner = 0; corner < 4; corner++) {
			float thisProjection = axis[i].dot(thisCorners[corner]);
			thisMin = std::min(thisMin, thisProjection);
			thisMax = std::max(thisMax, thisProjection);
		}
		if (circleMin> thisMax || thisMin > circleMax) {
			return false;
		}
	}
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
