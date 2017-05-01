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
	virtual bool doesCollide(Collider* other);

	// test collision with point
	virtual bool doesCollide(Vector2 point);

	// test collision with AABox
	virtual bool doesCollideWithAABox(AABox* box);
	virtual bool doesCollideWithOBox(OBox* box);
	virtual bool doesCollideWithCircle(CircleCollider* circle);

	// Accessor and mutator functions
	Vector2 getMinCorner() {
		return m_min;
	}

	Vector2 getMaxCorner() {
		return m_max;
	}

	void setCorners(Vector2 a, Vector2 b);

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

