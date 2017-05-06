#include <algorithm>
#include "AABox.h"
#include "OBox.h"
#include "CircleCollider.h"
#include "Ray.h"

AABox::AABox()
{
}

AABox::AABox(Vector2 cornerA, Vector2 cornerB)
{
	setCorners(cornerA, cornerB);
}


AABox::~AABox()
{
}

bool AABox::doesCollide(Collider * other)
{
	// Calls method on other collider to test collision with AABox
	return other->doesCollideWithAABox(this);
}

bool AABox::doesCollide(Vector2 point)
{
	if (point[0]<m_min[0] || point[0]>m_max[0] || point[1]<m_min[1] || point[1]>m_max[1]) {
		return false;
	}
	else {
		return true;
	}
}

bool AABox::doesCollideWithAABox(AABox * box)
{
	if (box->m_min[0] > m_max[0] || box->m_min[1] > m_max[1] || box->m_max[0] < m_min[0] || box->m_max[1] < m_min[1]) {
		return false;
	}
	else {
		return true;
	}
}

bool AABox::doesCollideWithOBox(OBox * box)
{
	box->doesCollideWithAABox(this);
	return false;
}

bool AABox::doesCollideWithCircle(CircleCollider * circle)
{
	return circle->doesCollideWithAABox(this);
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
