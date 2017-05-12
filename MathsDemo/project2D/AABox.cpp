#include <algorithm>
#include <tuple>
#include "AABox.h"
#include "OBox.h"
#include "CircleCollider.h"
#include "Ray.h"
#include "Plane.h"

AABox::AABox()
{
	setCorners({ 1,1 }, { -1,-1 });
}

AABox::AABox(Vector2 cornerA, Vector2 cornerB)
{
	setCorners(cornerA, cornerB);
}


AABox::~AABox()
{
}

std::pair<bool, Vector2> AABox::doesCollide(Collider * other)
{
	// Calls method on other collider to test collision with AABox
	std::pair<bool, Vector2> otherResult = other->doesCollideWithAABox(this);
	return std::make_pair(otherResult.first, otherResult.second * -1);	//Reverse penetration direction
}

std::pair<bool, Vector2> AABox::doesCollide(Vector2 point)
{
	if (point[0]<=m_min[0] || point[0]>=m_max[0] || point[1]<=m_min[1] || point[1]>=m_max[1]) {
		return std::make_pair( false, Vector2());
	}
	else {
		Vector2 penetration;
		float minX, minY;
		// Get smallest distance from left and right sides
		if (abs(point[0] - m_min[0]) < abs(point[0] - m_max[0])) {
			minX = point[0] - m_min[0];
		}
		else {
			minX = point[0] - m_max[0];
		}
		// Get smallest distance from top and bottom
		if (abs(point[1] - m_min[1]) < abs(point[1] - m_max[1])) {
			minY = point[1] - m_min[1];
		}
		else {
			minY = point[1] - m_max[1];
		}
		// If left/right side nearer, return vector pointing from that side
		if (abs(minX) < abs(minY)) {
			penetration[0] = minX;
			penetration[1] = 0;
		}	// Else return vector pointng from top/bottom
		else {
			penetration[0] = 0;
			penetration[1] = minY;
		}

	return	std::make_pair(true, penetration);
	}
}

std::pair<bool, Vector2> AABox::doesCollide(Plane plane)
{
	bool collision;
	// Get vector containing each corner of the box
	std::tuple<Vector2, Vector2, Vector2, Vector2> corners = getCorners();
	Vector2 cornerArr[4] = { std::get<0>(corners), std::get<1>(corners), std::get<2>(corners), std::get<3>(corners) };
	// Check each corner's distance to plane and get lowest (closest in front, or furthest behind)
	float minDistance = INFINITY;
	for (size_t i = 0; i < 4; ++i) {
		float distance = plane.distanceToPlane(cornerArr[i]);
		minDistance = std::min(minDistance, distance);
	}
	// If lowest distance is negative, box is at least partially behind plane
	collision = minDistance < 0;
	// Penetration behind plane is plane's normal times the distance behind it
	Vector2 penetration = plane.getNormal();
	penetration.normalise();
	penetration = -minDistance*penetration;
	return std::make_pair(collision, penetration);
}

std::pair<bool, Vector2> AABox::doesCollideWithAABox(AABox * box)
{
	// Check if boxes overlap
	if (box->m_min[0] >= m_max[0] ||
		box->m_min[1] >= m_max[1] ||
		box->m_max[0] <= m_min[0] ||
		box->m_max[1] <= m_min[1]) {
		// If box corners don't overlap, no collision
		return std::make_pair(false,Vector2());
	}
	else {
		// Calculate penetration vector
		Vector2 penetration;
		float minX, minY;
		// get smallest distance in x axis between max and min corners
		if (abs(box->m_min[0] - m_max[0]) < abs(box->m_max[0] - m_min[0])) {
			minX = box->m_min[0] - m_max[0];
		}
		else {
			minX = box->m_max[0] - m_min[0];
		}
		// get smallest distance in y axis between max and min corners
		if (abs(box->m_min[1] - m_max[1]) < abs(box->m_max[1] - m_min[1])) {
			minY = box->m_min[1] - m_max[1];
		} else{
			minY = box->m_max[1] - m_min[1];
		}
		// if distance in x axis is smaller, return vector pointing from that side
		if (abs(minX) < abs(minY)) {
			penetration[0] = minX;
			penetration[1] = 0;
		}
		else {
			// if distance in y axis is smaller, return vector pointing from that side
			penetration[0] = 0;
			penetration[1] = minY;
		}

	return	std::make_pair(true, penetration);
	}
}

std::pair<bool, Vector2> AABox::doesCollideWithOBox(OBox * box)
{
	// Call OBox's method to test collision with AABox
	std::pair<bool, Vector2> otherResult = box->doesCollideWithAABox(this);
	return std::make_pair(otherResult.first, otherResult.second * -1.0f);
}

std::pair<bool, Vector2> AABox::doesCollideWithCircle(CircleCollider * circle)
{
	// Call CircleCollider's method to test collision with AABox
	std::pair<bool, Vector2> otherResult = circle->doesCollideWithAABox(this);
	return std::make_pair(otherResult.first, otherResult.second * -1.0f);
}

bool AABox::isHitByRay(Ray * ray)
{
	return ray->doesCollide(this);
}

void AABox::setCorners(Vector2 a, Vector2 b)
{
	// Set m_min[0] and m_max[0]
	if (a[0] < b[0]) {
		m_min[0] = a[0];
		m_max[0] = b[0];
	}
	else {
		m_min[0] = b[0];
		m_max[0] = a[0];
	}
	// Set m_min[1] and m_max[1]
	if (a[1] < b[1]) {
		m_min[1] = a[1];
		m_max[1] = b[1];
	}
	else {
		m_min[1] = b[1];
		m_max[1] = a[1];
	}
}

std::tuple<Vector2, Vector2, Vector2, Vector2> AABox::getCorners()
{
	// Create vectors for corners between m_min and m_max
	Vector2 topLeft = { m_min[0],m_max[1] };
	Vector2 bottomRight = { m_min[1], m_max[0] };
	return std::make_tuple(m_min, topLeft, m_max, bottomRight);
}

void AABox::fitPoints(std::vector<Vector2> points)
{
	if (points.begin() == points.end()) {
		// TODO throw argument exception
	}
	m_min = Vector2(INFINITY, INFINITY);
	m_max = Vector2(-INFINITY, -INFINITY);
	for (std::vector<Vector2>::iterator it = points.begin(); it != points.end(); ++it) {
		// Set m_min's components to minimum among points
		m_min[0] = std::min(m_min[0], (*it)[0]);
		m_min[1] = std::min(m_min[1], (*it)[1]);
		// Set m_max's components to maximum among points
		m_max[0] = std::max(m_max[0], (*it)[0]);
		m_max[1] = std::max(m_max[1], (*it)[1]);
	}
}

void AABox::fitAABoxes(std::vector<AABox> boxes)
{
	if (boxes.begin() == boxes.end()) {
		// TODO throw argument exception
	}
	m_min = Vector2(INFINITY, INFINITY);
	m_max = Vector2(-INFINITY, -INFINITY);
	for (std::vector<AABox>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
		// Set m_min's components to minimum among m_min of boxes
		m_min[0] = std::min(m_min[0], it->m_min[0]);
		m_min[1] = std::min(m_min[1], it->m_min[1]);
		// Set m_max's components to maximum among m_max of boxes
		m_max[0] = std::max(m_max[0], it->m_max[0]);
		m_max[1] = std::max(m_max[1], it->m_max[1]);
	}
}

void AABox::addPoints(std::vector<Vector2> points)
{
	for (std::vector<Vector2>::iterator it = points.begin(); it != points.end(); ++it) {
		m_min[0] = std::min(m_min[0], (*it)[0]);
		m_min[1] = std::min(m_min[1], (*it)[1]);
		m_max[0] = std::max(m_max[0], (*it)[0]);
		m_max[1] = std::max(m_max[1], (*it)[1]);
	}
}

void AABox::addAABoxes(std::vector<AABox> boxes)
{
	for (std::vector<AABox>::iterator it = boxes.begin(); it != boxes.end(); ++it) {
		m_min[0] = std::min(m_min[0], it->m_min[0]);
		m_min[1] = std::min(m_min[1], it->m_min[1]);
		m_max[0] = std::max(m_max[0], it->m_max[0]);
		m_max[1] = std::max(m_max[1], it->m_max[1]);
	}
}
