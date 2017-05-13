#include "CircleCollider.h"
#include <algorithm>
#include "AABox.h"
#include "OBox.h"
#include "Ray.h"
#include "Plane.h"

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
	// Call other collider's method for colliding with CircleCollider
	std::pair<bool, Vector2> otherResult = other->doesCollideWithCircle(this);
	return std::make_pair(otherResult.first, otherResult.second * -1);			// Reverse direction of penetration
}

std::pair<bool, Vector2> CircleCollider::doesCollide(Vector2 point)
{
	// Get vector from centre to point
	Vector2 displacement = point - m_centre;
	// Check if displacement is less than radius
	if (displacement.magnitudeSquared() >= (m_radius*m_radius)) {
		// If displacement greater or equal, no collision
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

std::pair<bool, Vector2> CircleCollider::doesCollide(Plane plane)
{
	Vector2 penetration = plane.getNormal();
	penetration.normalise();
	// Get distance of point furthest behind/closest in front of plane
	float distance = plane.distanceToPlane(m_centre) - m_radius;
	// If distance is negative, point must be behind plane
	bool collision = distance < 0;
	penetration = penetration * -distance;
	return std::make_pair(collision,penetration);
}

std::pair<bool, Vector2> CircleCollider::doesCollideWithAABox(AABox * box)
{
	// Check if circle's centre is inside box
	std::pair<bool, Vector2> collision = box->doesCollide(m_centre);
	if (collision.first) {
		// If so, get penetration of centre into box
		Vector2 penetration = collision.second;
		// Add radius to penetration's magnitude
		Vector2 penNormal = penetration;
		penNormal.normalise();
		penetration += m_radius*penNormal;
		return std::make_pair(true, -1.0f * penetration);
	}
	else {
		// Find closest point on box to circle
		float clampedX, clampedY;
		// x value clamped between max and min corners
		clampedX = std::min(std::max(m_centre[0], box->getMinCorner()[0]), box->getMaxCorner()[0]);
		// y value clamped between max and min corners
		clampedY = std::min(std::max(m_centre[1], box->getMinCorner()[1]), box->getMaxCorner()[1]);
		Vector2 closestPoint = Vector2(clampedX, clampedY);
		// Test collision with closest point
		return doesCollide(closestPoint);
	}
}

std::pair<bool, Vector2> CircleCollider::doesCollideWithOBox(OBox * box)
{
	// Call OBox's method to test collision with CircleCollider
	std::pair<bool, Vector2> otherResult = box->doesCollideWithCircle(this);
	return std::make_pair(otherResult.first, otherResult.second * -1);
}

std::pair<bool, Vector2> CircleCollider::doesCollideWithCircle(CircleCollider * circle)
{
	Vector2 displacement = circle->m_centre - m_centre;
	// Check if distance between centres is less than sum of circle radii
	if (displacement.compareMagnitude(m_radius + circle->m_radius) == -1) {
		// Get nearest point at edge of circle to centre of other circle
		Vector2 nearestEdge, otherNearestEdge;
		Vector2 direction = displacement;
		if (!direction.normalise()) {
			//If normalisation fails (possibly because centres are too close), arbitrarily set direction to {1,0}
			direction = Vector2(1, 0);
		}
		// Nearest point is at m_radius distance along direction
		nearestEdge = m_centre + m_radius * direction ;
		otherNearestEdge = circle->m_centre - circle->m_radius * direction;
		// Penetration vector is difference between these edges
		return std::make_pair(true, otherNearestEdge - nearestEdge);
	} else {
		// If distance between centres is greater, no collision
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

	// Set radius as distance to furthest point
	float maxDistanceSquared = 0.0f;
	for (std::vector<Vector2>::iterator it = points.begin(); it != points.end(); ++it) {
		maxDistanceSquared = std::max(maxDistanceSquared, (*it - m_centre).magnitudeSquared());
	}
	m_radius = sqrtf(maxDistanceSquared);

		//TODO shrink circle until 2 points at edge
	//TODO shrink circle until points have 180 degree arc/3 points at edge
}
