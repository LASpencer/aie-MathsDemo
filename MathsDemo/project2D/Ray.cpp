#include <algorithm>
#include "Ray.h"
#include "Collider.h"
#include "AABox.h"
#include "CircleCollider.h"
#include "OBox.h"
#include <exception>


Ray::Ray() : m_origin({ 0,0 }), m_direction({ 1,0 })
{
}

Ray::Ray(Vector2 origin, Vector2 direction) : m_origin(origin)
{
	bool valid = direction.normalise();
	if (!valid){
		throw new std::invalid_argument("Ray direction could not be normalised");
	}
	m_direction = direction;
}


Ray::~Ray()
{
}

void Ray::setOrigin(Vector2 origin)
{
	m_origin = origin;
}

void Ray::setDirection(Vector2 direction)
{
	bool valid = direction.normalise();
	if (!valid) {
		throw new std::invalid_argument("Ray direction could not be normalised");
	}
	m_direction = direction;

}

Vector2 Ray::findClosestPoint(Vector2 point)
{
	Vector2 displacement = point - m_origin;
	float projection = displacement.dot(m_direction);
	projection = std::max(projection, 0.0f);
	Vector2 closestPoint = m_origin +  m_direction * projection;
	return closestPoint;
}

bool Ray::doesCollide(Collider * collider)
{
	// Collider's isHitByRay method will call doesCollide with pointer of concrete type
	return collider->isHitByRay(this);
}

bool Ray::doesCollide(CircleCollider* circle)
{
	// Does closest point on ray to centre of circle collide with the circle?
	Vector2 displacement = findClosestPoint(circle->getCentre()) - circle->getCentre();
	// If displacement is less than circle radius, collision occured
	if (displacement.compareMagnitude(circle->getRadius()) == -1) {
		return true;
	} else{
		return false;
	}
}

bool Ray::doesCollide(AABox* box)
{
	float dEntry = 0.0f;	// Distance from ray where point would enter if interesecting.
	float dExit = INFINITY;	// Distance from ray where point would exit if intersecting
	if (m_direction[0] != 0.0f) {
		float xRate = 1.0f / m_direction[0];
		// get distance from ray where x component is equal to corners
		float dx1 = (box->getMinCorner()[0] - m_origin[0])*xRate;
		float dx2 = (box->getMaxCorner()[0] - m_origin[0])*xRate;
		// Entry distance is lowest x distance greater than 0 (if origin within or past box, remains 0)
		dEntry = std::max(dEntry, std::min(dx1, dx2));
		// Exit distance is greatest x distance. If negative, ray points away from box in x axis
		dExit = std::min(dExit, std::max(dx1, dx2));
	}
	else {
		// if ray is vertical, check if origin x component between corner x components
		if (m_origin[0] < box->getMinCorner()[0] || m_origin[0] > box->getMaxCorner()[0]) {
			// If not, ray cannot intersect with box
			return false;
		}	// Else, intersection can be easily checked based on ray's direction
		else if (m_direction[1] > 0.0f) {
			// If direction positive, ray intersects if origin beneath top of box
			return m_origin[1] <= box->getMaxCorner()[1];
		}
		else {
			// If direction negative, ray intersects if origin above bottom of box
			return m_origin[1] >= box->getMinCorner()[1];
		}
	}
	if (m_direction[1] != 0.0f) {
		float yRate = 1.0f / m_direction[1];
		// get distance from ray where x component is equal to corners
		float dy1 = (box->getMinCorner()[1] - m_origin[1])*yRate;
		float dy2 = (box->getMaxCorner()[1] - m_origin[1])*yRate;
		// Entry distance is greatest of: lowest y distance, lowest x distance, 0
		// So minimum distance before a corner is passed, or 0 if origin in box or ray points away from box
		dEntry = std::max(dEntry, std::min(dy1, dy2));
		// Exit distance is lowest of: greatest y distance, greatest x distance
		// So minimum distance before both corners are passed. If negative, ray points away from box
		dExit = std::min(dExit, std::max(dy1, dy2));
	}
	else {
		// if ray is horizontal, check if origin y component between corner y components
		if (m_origin[1] < box->getMinCorner()[1] || m_origin[1] > box->getMaxCorner()[1]) {
			return false;
		}
	}
	// If entry less than exit, ray hits a side of the box. If equal origin is on an edge
	// If entry greater, ray misses box (or is pointing away from it)
	return dEntry <= dExit;
}

bool Ray::doesCollide(OBox * box)
{
	// Get inverse transformation of OBox
	Matrix3 inverseOBox = box->getInverseTransform();
	// Create copy of ray transformed by this matrix
	Vector3 transformedDirection = inverseOBox * (Vector3)m_direction;
	Vector3 transformedOrigin = (Vector3)m_origin;
	transformedOrigin[2] = 1.0f;
	transformedOrigin = inverseOBox * transformedOrigin;
	Ray transformedRay((Vector2)transformedOrigin, (Vector2)transformedDirection);
	// Check transformed ray against AABox with corners (-1,-1) and (1,1)
	AABox transformedBox({ -1,-1 }, { 1,1 });
	return transformedRay.doesCollide(&transformedBox);
}
