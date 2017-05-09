#include "OBox.h"
#include "AABox.h"
#include "CircleCollider.h"
#include "Ray.h"
#include "Plane.h"

OBox::OBox() : m_xExtent({1,0}), m_yExtent({0,1}), m_centre()
{
}

OBox::OBox(Vector2 xExtent, Vector2 yExtent, Vector2 centre) : m_xExtent(xExtent), m_yExtent(yExtent), m_centre(centre)
{
	//TODO throw exception if invalid vectors
	//TODO rewrite so arguments are centre, scale, angle
}


OBox::~OBox()
{
}

std::pair<bool, Vector2> OBox::doesCollide(Collider * other)
{
	std::pair<bool, Vector2> otherResult = other->doesCollideWithOBox(this);
	return std::make_pair(otherResult.first, otherResult.second * -1);
}

std::pair<bool, Vector2> OBox::doesCollide(Vector2 point)
{
	Vector2 displacement = point - m_centre;
	float xProjection = m_xExtent.dot(displacement);
	float yProjection = m_yExtent.dot(displacement);
	// If dot product of displacement and extent is less than the extent's magnitude, point lies within the box
	if (m_xExtent.compareMagnitude(xProjection) == 1 && m_yExtent.compareMagnitude(yProjection) == 1) {
		// Calculate vectors along extent equal in magnitude to projection of point on extent
		Vector2 xDisplacement = m_xExtent;
		Vector2 yDisplacement = m_yExtent;
		Vector2 xPenetration, yPenetration, minPenetration;
		xDisplacement.normalise();
		yDisplacement.normalise();
		xDisplacement = xDisplacement * xProjection;
		yDisplacement = yDisplacement * yProjection;
		// i
		if (xProjection >= 0) {
			xPenetration = xDisplacement - m_xExtent;
		} else{
			xPenetration = xDisplacement + m_xExtent;
		}
		if (yProjection >= 0) {
			yPenetration = yDisplacement - m_yExtent;
		}
		else {
			yPenetration = yDisplacement + m_yExtent;
		}
		if (xPenetration < yPenetration) {
			minPenetration = xPenetration;
		}
		else {
			minPenetration = yPenetration;
			}
		//return vector along chosen extent, pointing inward
		// Check penetration vector isn't zero
		bool penetrationNotZero = !(minPenetration.isZeroVector());
		return std::make_pair(penetrationNotZero, minPenetration);
	} else {
		return std::make_pair(false, Vector2());
	}
}

std::pair<bool, Vector2> OBox::doesCollide(Plane plane)
{
	bool collision;
	std::tuple<Vector2, Vector2, Vector2, Vector2> corners = getCorners();
	Vector2 cornerArr[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };
	float minDistance = INFINITY;
	for (size_t i = 0; i < 4; ++i) {
		float distance = plane.distanceToPlane(cornerArr[i]);
		minDistance = std::min(minDistance, distance);
	}
	collision = minDistance < 0;
	Vector2 penetration = plane.getNormal();
	penetration.normalise();
	penetration = -minDistance*penetration;
	return std::make_pair(collision, penetration);
}

std::pair<bool, Vector2> OBox::doesCollideWithAABox(AABox * box)
{
	//TODO tests
	// get each corner of the boxes
	Vector2 thisCorners[4];
	Vector2 otherCorners[4];
	thisCorners[0] = m_centre + m_xExtent + m_yExtent;
	thisCorners[1] = m_centre + m_xExtent - m_yExtent;
	thisCorners[2] = m_centre - m_xExtent - m_yExtent;
	thisCorners[3] = m_centre - m_xExtent + m_yExtent;

	Vector2 minCorner = box->getMinCorner();
	Vector2 maxCorner = box->getMaxCorner();

	otherCorners[0] = minCorner;
	otherCorners[1] = Vector2(minCorner[0],maxCorner[1]);
	otherCorners[2] = maxCorner;
	otherCorners[3] = Vector2(maxCorner[0],minCorner[1]);

	// get each axis to test
	Vector2 axis[8];
	Vector2 minAxis;
	float minOverlap = INFINITY;
	for (size_t i = 0; i < 4; i++) {
		Vector2 thisSide, otherSide;
		if (i == 3) {
			// Side from last corner to first
			thisSide = thisCorners[0] - thisCorners[3];
			otherSide = otherCorners[0] - otherCorners[3];
		}
		else {
			// Side from current corner to next
			thisSide = thisCorners[i + 1] - thisCorners[i];
			otherSide = otherCorners[i + 1] - otherCorners[i];
		}
		// Normalise sides
		thisSide.normalise();
		otherSide.normalise();
		// Axis is normal of side
		axis[i][0] = -(thisSide[1]);
		axis[i][1] = thisSide[0];
		axis[i + 4][0] = -(otherSide[1]);
		axis[i + 4][1] = otherSide[0];
	}
	// Project boxes onto each axis and check for overlap
	for (size_t i = 0; i < 8; i++) {
		float thisMin = INFINITY;
		float otherMin = INFINITY;
		float thisMax = -INFINITY;
		float otherMax = -INFINITY;
		for (size_t corner = 0; corner < 4; corner++) {
			float thisProjection = axis[i].dot(thisCorners[corner]);
			float otherProjection = axis[i].dot(otherCorners[corner]);
			thisMin = std::min(thisMin, thisProjection);
			thisMax = std::max(thisMax, thisProjection);
			otherMin = std::min(otherMin, otherProjection);
			otherMax = std::max(otherMax, otherProjection);
		}
		float overlap = std::min(thisMax - otherMin, otherMax - thisMin);
		if (overlap <= 0) {
			return std::make_pair(false, Vector2());
		}
		else {
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minAxis = axis[i];
			}
		}
	}
	Vector2 minPenetration = minOverlap * minAxis;
	Vector2 otherCentre = 0.5*(minCorner + maxCorner);
	Vector2 translationDirection = m_centre - otherCentre;
	if (minPenetration.dot(translationDirection) < 0) {
		minPenetration = -1 * minPenetration;
	}
	return std::make_pair(true, minPenetration);
}

std::pair<bool, Vector2> OBox::doesCollideWithOBox(OBox * box)
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
	Vector2 minAxis;
	float minOverlap = INFINITY;
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
		// Normalise sides
		thisSide.normalise();
		otherSide.normalise();
		// Axis is normal of side
		axis[i][0] = -(thisSide[1]);
		axis[i][1] = thisSide[0];
		axis[i + 4][0] = -(otherSide[1]);
		axis[i + 4][1] = otherSide[0];
	}
	// Project boxes onto each axis and check for overlap
	for (size_t i = 0; i < 8; i++) {
		float thisMin = INFINITY; 
		float otherMin = INFINITY;
		float thisMax = -INFINITY;
		float otherMax = -INFINITY;
		for (size_t corner = 0; corner < 4; corner++) {
			float thisProjection = axis[i].dot(thisCorners[corner]);
			float otherProjection = axis[i].dot(otherCorners[corner]);
			thisMin = std::min(thisMin, thisProjection);
			thisMax = std::max(thisMax, thisProjection);
			otherMin = std::min(otherMin, otherProjection);
			otherMax = std::max(otherMax, otherProjection);
		}
		float overlap = std::min(thisMax - otherMin, otherMax - thisMin);
		if (overlap<=0) {
			return std::make_pair(false, Vector2());
		} else{
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minAxis = axis[i];
			}
		}
	}
	Vector2 minPenetration = minOverlap * minAxis;
	Vector2 translationDirection = m_centre - box->m_centre;
	if (minPenetration.dot(translationDirection) < 0) {
		minPenetration = -1 * minPenetration;
	}
	return std::make_pair(true,minPenetration);
}

std::pair<bool, Vector2> OBox::doesCollideWithCircle(CircleCollider * circle)
{
	//TODO test
	Vector2 thisCorners[4];
	thisCorners[0] = m_centre + m_xExtent + m_yExtent;
	thisCorners[1] = m_centre + m_xExtent - m_yExtent;
	thisCorners[2] = m_centre - m_xExtent - m_yExtent;
	thisCorners[3] = m_centre - m_xExtent + m_yExtent;
	// get each axis to test
	Vector2 axis[4];
	Vector2 minAxis;
	float minOverlap = INFINITY;
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
		// Normalise sides
		thisSide.normalise();
		// Axis is normal of side
		axis[i][0] = -(thisSide[1]);
		axis[i][1] = thisSide[0];
	}
	// Project boxe and circle onto each axis and check for overlap
	for (size_t i = 0; i < 4; i++) {
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
		float overlap = std::min(thisMax - circleMin, circleMax - thisMin);
		if (overlap<=0) {
			return std::make_pair(false, Vector2());
		} else{
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minAxis = axis[i];
			}
		}
	}
	Vector2 minPenetration = minOverlap * minAxis;
	Vector2 translationDirection = m_centre - circle->getCentre();
	if (minPenetration.dot(translationDirection) < 0) {
		minPenetration = -1 * minPenetration;
	}
	return std::make_pair(true, minPenetration);
}

bool OBox::isHitByRay(Ray * ray)
{
	return ray->doesCollide(this);
}

std::tuple<Vector2, Vector2, Vector2, Vector2> OBox::getCorners()
{
	Vector2 thisCorners[4];
	thisCorners[0] = m_centre + m_xExtent + m_yExtent;
	thisCorners[1] = m_centre + m_xExtent - m_yExtent;
	thisCorners[2] = m_centre - m_xExtent - m_yExtent;
	thisCorners[3] = m_centre - m_xExtent + m_yExtent;
	return std::make_tuple(thisCorners[0], thisCorners[1], thisCorners[2], thisCorners[3]);
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

void OBox::transform(Matrix3 matrix)
{
	//TODO
}

void OBox::fitPoints(std::vector<Vector2> points)
{
	//TODO
}

Matrix3 OBox::getInverseTransform()
{
	//TODO possible optimization: memoize result, recalculate if obox changes (set dirty flag)
	Matrix3 inverse;
	getBoxMatrix().calculateInverse(inverse);
	return inverse;
}
