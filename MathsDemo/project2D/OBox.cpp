#include <vector>
#include "Vector.h"
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
}


OBox::~OBox()
{
}

std::pair<bool, Vector2> OBox::doesCollide(Collider * other)
{
	// Call other collider's method for collision with OBox
	std::pair<bool, Vector2> otherResult = other->doesCollideWithOBox(this);
	return std::make_pair(otherResult.first, otherResult.second * -1);
}

std::pair<bool, Vector2> OBox::doesCollide(Vector2 point)
{
	Vector2 displacement = point - m_centre;
	// Get projection of displacement to point along each extent
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
		// Set x and y penetration as vectors from edge to the point
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
		// Pick penetration vector with smallest magnitude
		if (xPenetration < yPenetration) {
			minPenetration = xPenetration;
		}
		else {
			minPenetration = yPenetration;
			}
		// Check penetration vector isn't zero (point is on edge)
		bool penetrationNotZero = !(minPenetration.isZeroVector());
		// Return true if penetration vector is not zero vector
		return std::make_pair(penetrationNotZero, minPenetration);
	} else {
		// If point lies beyond both extents, return false
		return std::make_pair(false, Vector2());
	}
}

std::pair<bool, Vector2> OBox::doesCollide(Plane plane)
{
	bool collision;
	// Get vector containing each corner of the box
	std::tuple<Vector2, Vector2, Vector2, Vector2> corners = getCorners();
	Vector2 cornerArr[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };
	float minDistance = INFINITY;
	// Check each corner's distance to plane and get lowest (closest in front, or furthest behind)
	for (size_t i = 0; i < 4; ++i) {
		float distance = plane.distanceToPlane(cornerArr[i]);
		minDistance = std::min(minDistance, distance);
	}
	// If losest distance is negative, box is at least partially behind plane
	collision = minDistance < 0;
	// Penetration behind plane is plane's normal times the distance behind it
	Vector2 penetration = plane.getNormal();
	penetration.normalise();
	penetration = -minDistance*penetration;
	return std::make_pair(collision, penetration);
}

std::pair<bool, Vector2> OBox::doesCollideWithAABox(AABox * box)
{
	// get each corner of both boxes
	std::tuple<Vector2, Vector2, Vector2, Vector2> corners = getCorners();
	Vector2 thisCorners[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };
	corners = box->getCorners();
	Vector2 otherCorners[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };

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
		// Project each point onto axis and record minimum and maximum values for each box
		for (size_t corner = 0; corner < 4; corner++) {
			float thisProjection = axis[i].dot(thisCorners[corner]);
			float otherProjection = axis[i].dot(otherCorners[corner]);
			thisMin = std::min(thisMin, thisProjection);
			thisMax = std::max(thisMax, thisProjection);
			otherMin = std::min(otherMin, otherProjection);
			otherMax = std::max(otherMax, otherProjection);
		}
		float overlap = std::min(thisMax - otherMin, otherMax - thisMin);
		// Check if overlap exists on this axis
		if (overlap <= 0) {
			// If no overlap, boxes do not collide
			return std::make_pair(false, Vector2());
		}
		else {
			// Check if this is axis with lowest overlap so far
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minAxis = axis[i];
			}
		}
	}
	// If no axis without overlap was found, collision must have occured
	// Penetration vector is axis with minimum overlap, multiplied by that overlap
	Vector2 minPenetration = minOverlap * minAxis;
	// Calculate centre of AABox
	Vector2 otherCentre = 0.5*(box->getMinCorner() + box->getMaxCorner());
	// Make sure penetration vector is pointing away from other box's centre
	Vector2 translationDirection = m_centre - otherCentre;
	if (minPenetration.dot(translationDirection) < 0) {
		minPenetration = -1 * minPenetration;
	}
	return std::make_pair(true, minPenetration);
}

std::pair<bool, Vector2> OBox::doesCollideWithOBox(OBox * box)
{
	// get each corner of both boxes
	std::tuple<Vector2, Vector2, Vector2, Vector2> corners = getCorners();
	Vector2 thisCorners[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };
	corners = box->getCorners();
	Vector2 otherCorners[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };

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
		// Project each point onto axis and record minimum and maximum values for each box
		for (size_t corner = 0; corner < 4; corner++) {
			float thisProjection = axis[i].dot(thisCorners[corner]);
			float otherProjection = axis[i].dot(otherCorners[corner]);
			thisMin = std::min(thisMin, thisProjection);
			thisMax = std::max(thisMax, thisProjection);
			otherMin = std::min(otherMin, otherProjection);
			otherMax = std::max(otherMax, otherProjection);
		}
		float overlap = std::min(thisMax - otherMin, otherMax - thisMin);
		// Check if overlap exists on this axis
		if (overlap<=0) {
			// If no overlap, boxes do not collide
			return std::make_pair(false, Vector2());
		} else{
			// Check if this is axis with lowest overlap so far
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minAxis = axis[i];
			}
		}
	}
	// If no axis without overlap was found, collision must have occured
	// Penetration vector is axis with minimum overlap, multiplied by that overlap
	Vector2 minPenetration = minOverlap * minAxis;
	// Make sure penetration vector is pointing away from other box's centre
	Vector2 translationDirection = m_centre - box->m_centre;
	if (minPenetration.dot(translationDirection) < 0) {
		minPenetration = -1 * minPenetration;
	}
	return std::make_pair(true,minPenetration);
}

std::pair<bool, Vector2> OBox::doesCollideWithCircle(CircleCollider * circle)
{
	// Get corners of this box
	std::tuple<Vector2, Vector2, Vector2, Vector2> corners = getCorners();
	Vector2 thisCorners[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };

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
	// Project box and circle onto each axis and check for overlap
	for (size_t i = 0; i < 4; i++) {
		float thisMin= INFINITY;
		float thisMax= -INFINITY;
		// Circle projection will always be projection of centre +/- radius
		float circleCentre = axis[i].dot(circle->getCentre());
		float circleMin = circleCentre - circle->getRadius();
		float circleMax = circleCentre + circle->getRadius();
		// Project each point onto axis and record minimum and maximum values for each box
		for (size_t corner = 0; corner < 4; corner++) {
			float thisProjection = axis[i].dot(thisCorners[corner]);
			thisMin = std::min(thisMin, thisProjection);
			thisMax = std::max(thisMax, thisProjection);
		}
		float overlap = std::min(thisMax - circleMin, circleMax - thisMin);
		// Check if overlap exists on this axis
		if (overlap<=0) {
			// If no overlap, box and circle do not collide
			return std::make_pair(false, Vector2());
		} else{
			// Check if this is axis with lowest overlap so far
			if (overlap < minOverlap) {
				minOverlap = overlap;
				minAxis = axis[i];
			}
		}
	}
	// If no axis without overlap was found, collision must have occured
	// Penetration vector is axis with minimum overlap, multiplied by that overlap
	Vector2 minPenetration = minOverlap * minAxis;
	// Make sure penetration vector is pointing away from circle's centre
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

void OBox::fitPoints(std::vector<Vector2> points)
{
	if (points.begin() == points.end()) {
		throw std::invalid_argument("Cannot pass empty vector to fitPoints");
	}
	// Set centre as mean value of points
	float meanX = 0;
	float meanY = 0;
	for (std::vector<Vector2>::iterator it = points.begin(); it != points.end(); ++it) {
		meanX += (*it)[0];
		meanY += (*it)[1];
	}
	meanX /= points.size();
	meanY /= points.size();
	m_centre = { meanX,meanY };
	// Find vector to furthest point from centre
	std::vector<Vector2> displacement;
	Vector2 xExtent;
	float maxMagSquared = 0;
	for (std::vector<Vector2>::iterator it = points.begin(); it != points.end(); ++it) {
		// Get difference between centre and point
		Vector2 difference = (*it) - m_centre;
		displacement.push_back(difference);
		float magSquared = difference.magnitudeSquared();
		if (magSquared > maxMagSquared) {
			// If difference greater than current maximum, this is new maximum
			xExtent = difference;
			maxMagSquared = magSquared;
		}
	}
	if (maxMagSquared == 0) {
		// If furthest point from centre is at centre, set extents as very small values
		m_xExtent = { FLT_EPSILON * 64,0 };
		m_yExtent = { 0,FLT_EPSILON *64};
	}
	else {
		// Set xExtent
		m_xExtent = xExtent;
		// Get a normal vector perpendicular to xExtent
		Vector2 yExtent = { -m_xExtent[1],m_xExtent[0] };
		yExtent.normalise();
		// Find greatest projection along this vector
		float maxProjection = 0;
		for (std::vector<Vector2>::iterator it = displacement.begin(); it != displacement.end(); ++it) {
			float projection = it->dot(yExtent);
			if (abs(projection) > abs(maxProjection)) {
				// If absolute value greater than current value, set as new maximum
				maxProjection = projection;
			}
		}
		if (maxProjection == 0) {
			// Set yExtent as very small vector orthogonal to xExtent
			m_yExtent = (FLT_EPSILON * 64)*yExtent;
		}
		else {
			// Set yExtent as maximum projection along vector orthogonal to xExtent
			m_yExtent = maxProjection * yExtent;
		}
	}
}

Matrix3 OBox::getInverseTransform()
{
	Matrix3 inverse;
	getBoxMatrix().calculateInverse(inverse);
	return inverse;
}
