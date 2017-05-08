#include "CircleCollider.h"
#include <algorithm>
#include "AABox.h"
#include "OBox.h"
#include "Ray.h"

CircleCollider::CircleCollider()
{
}

CircleCollider::CircleCollider(Vector2 centre, float radius) : m_centre(centre), m_radius(radius)
{
}


CircleCollider::~CircleCollider()
{
}

std::pair<bool, Vector2> CircleCollider::doesCollide(Collider * other)
{
	std::pair<bool, Vector2> otherResult = other->doesCollideWithCircle(this);
	return std::make_pair(otherResult.first, otherResult.second * -1);
}

std::pair<bool, Vector2> CircleCollider::doesCollide(Vector2 point)
{
	Vector2 displacement = point - m_centre;
	if (displacement.magnitudeSquared() >= (m_radius*m_radius)) {
		return std::make_pair(false,Vector2());
	}
	else {
		Vector2 nearestEdge;
		// Normalise displacement and multiply by radius to get nearest point on edge
		if (displacement.normalise()) {
			nearestEdge = m_centre + displacement * m_radius;
		}	// if point is at centre, arbitrarily pick a vector of magnitude m_radius
		else {
			nearestEdge = m_centre + Vector2(m_radius, 0);
			}
		// Return vector from nearest point on edge to point
		return std::make_pair(true, point - nearestEdge);
	}
}

std::pair<bool, Vector2> CircleCollider::doesCollideWithAABox(AABox * box)
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

std::pair<bool, Vector2> CircleCollider::doesCollideWithOBox(OBox * box)
{
	std::pair<bool, Vector2> otherResult = box->doesCollideWithCircle(this);
	return std::make_pair(otherResult.first, otherResult.second * -1);
}

std::pair<bool, Vector2> CircleCollider::doesCollideWithCircle(CircleCollider * circle)
{
	Vector2 displacement = circle->m_centre - m_centre;
	if (displacement.compareMagnitude(m_radius + circle->m_radius) == -1) {
		Vector2 nearestEdge, otherNearestEdge;
		Vector2 direction = displacement;
		// Normalise direction
		if (!direction.normalise()) {
			//If normalisation fails, arbitrarily set direction to {1,0}
			direction = Vector2(1, 0);
		}
		nearestEdge = m_centre + m_radius * direction ;
		otherNearestEdge = circle->m_centre - circle->m_radius * direction;
		return std::make_pair(true, otherNearestEdge - nearestEdge);
	}	else {
		return std::make_pair(false, Vector2());
	}
}

bool CircleCollider::isHitByRay(Ray * ray)
{
	return ray->doesCollide(this);
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
