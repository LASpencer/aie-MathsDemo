#pragma once
#include "Collider.h"
#include <vector>

class AABox :
	public Collider
{
public:
	AABox();
	AABox(Vector2 cornerA, Vector2 cornerB);
	~AABox();

	// test collision with Collider of unknown concrete type
	virtual std::pair<bool, Vector2> doesCollide(Collider* other);

	// test collision with point
	virtual std::pair<bool, Vector2> doesCollide(Vector2 point);

	// test collision with plane
	virtual std::pair<bool, Vector2> doesCollide(Plane plane);

	// test collision with AABox
	virtual std::pair<bool, Vector2> doesCollideWithAABox(AABox* box);
	virtual std::pair<bool, Vector2> doesCollideWithOBox(OBox* box);
	virtual std::pair<bool, Vector2> doesCollideWithCircle(CircleCollider* circle);

	// test collision with Ray
	virtual bool isHitByRay(Ray* ray);

	// Accessor and mutator functions
	Vector2 getMinCorner() {
		return m_min;
	}

	Vector2 getMaxCorner() {
		return m_max;
	}

	void setCorners(Vector2 a, Vector2 b);

	// Returns all four corners of the AABox, clockwise from minimum corner
	std::tuple<Vector2, Vector2, Vector2, Vector2> getCorners();

	// Fit AABox around points given
	void fitPoints(std::vector<Vector2> points);

	// Fit AABox around given AABoxes
	void fitAABoxes(std::vector<AABox> boxes);

	// Extend AABox to include points given
	void addPoints(std::vector<Vector2> points);

	// Extend AABox to include AABox given
	void addAABoxes(std::vector<AABox> boxes);

protected:
	Vector2 m_min;
	Vector2 m_max;
};

