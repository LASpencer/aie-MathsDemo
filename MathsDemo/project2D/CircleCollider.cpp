#include "CircleCollider.h"
#include <algorithm>
#include "AABox.h"


CircleCollider::CircleCollider()
{
}

CircleCollider::CircleCollider(Vector2 centre, float radius) : m_centre(centre), m_radius(radius)
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
	// Find closest point on box to circle
	float clampedX, clampedY;
	// x value clamped between max and min corners
	clampedX = std::max(m_centre[0], box->getMinCorner()[0]);
	clampedX = std::min(clampedX, box->getMaxCorner()[0]);
	// y value clamped between max and min corners
	clampedY = std::max(m_centre[1], box->getMinCorner()[1]);
	clampedY = std::min(clampedY, box->getMaxCorner()[1]);
	// check if nearest point is within circle
	return doesCollide(Vector2(clampedX, clampedY));
}

bool CircleCollider::doesCollideWithOBox(OBox * box)
{
	//TODO when OBox written call its doesCollideWithCircle method
	return false;
}

bool CircleCollider::doesCollideWithCircle(CircleCollider * circle)
{
	Vector2 displacement = circle->m_centre - m_centre;
	if (displacement.compareMagnitude(m_radius + circle->m_radius) == 1) {
		return false;
	} else {
		return true;
	}
}

void CircleCollider::fitPoints(std::vector<Vector2> points)
{
	if (points.begin() == points.end()) {
		//TODO throw exception
	}
	// Set radius as mean value of points
	float meanX = 0;
	float meanY = 0;
	for (std::vector<Vector2>::iterator it = points.begin(); it != points.end(); ++it) {
		meanX += (*it)[0];
		meanY += (*it)[1];
	}
	meanX /= points.size();
	meanY /= points.size();

	m_centre = { meanX,meanY };

	float maxDistanceSquared = 0.0f;
	for (std::vector<Vector2>::iterator it = points.begin(); it != points.end(); ++it) {
		maxDistanceSquared = std::max(maxDistanceSquared, (*it - m_centre).magnitudeSquared());
	}
	m_radius = sqrtf(maxDistanceSquared);

		//TODO shrink circle until 2 points at edge
	//TODO shrink circle until points have 180 degree arc/3 points at edge
}

void CircleCollider::fitCircle(std::vector<CircleCollider> circles)
{
	//TODO
}
