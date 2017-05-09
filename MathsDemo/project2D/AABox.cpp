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
	return std::make_pair(otherResult.first, otherResult.second * -1);
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

std::pair<bool, Vector2> AABox::doesCollideWithAABox(AABox * box)
{
	if (box->m_min[0] >= m_max[0] || box->m_min[1] >= m_max[1] || box->m_max[0] <= m_min[0] || box->m_max[1] <= m_min[1]) {

		return std::make_pair(false,Vector2());
	}
	else {
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
			penetration[0] = 0;
			penetration[1] = minY;
		}

	return	std::make_pair(true, penetration);
	}
}

std::pair<bool, Vector2> AABox::doesCollideWithOBox(OBox * box)
{
	std::pair<bool, Vector2> otherResult = box->doesCollideWithAABox(this);
	return std::make_pair(otherResult.first, otherResult.second * -1.0f);
}

std::pair<bool, Vector2> AABox::doesCollideWithCircle(CircleCollider * circle)
{
	std::pair<bool, Vector2> otherResult = circle->doesCollideWithAABox(this);
	return std::make_pair(otherResult.first, otherResult.second * -1.0f);
}

bool AABox::isHitByRay(Ray * ray)
{
	return ray->doesCollide(this);
}

void AABox::setCorners(Vector2 a, Vector2 b)
{
	if (a[0] < b[0]) {
		m_min[0] = a[0];
		m_max[0] = b[0];
	}
	else {
		m_min[0] = b[0];
		m_max[0] = a[0];
	}
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
		m_min[0] = std::min(m_min[0], (*it)[0]);
		m_min[1] = std::min(m_min[1], (*it)[1]);
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
		m_min[0] = std::min(m_min[0], it->m_min[0]);
		m_min[1] = std::min(m_min[1], it->m_min[1]);
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
