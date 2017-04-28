#pragma once
#include "Collider.h"
class AABox :
	public Collider
{
public:
	AABox();
	AABox(Vector2 min, Vector2 max);
	~AABox();

	// test collision with Collider of unknown concrete type
	virtual bool doesCollide(Collider* other);

	// test collision with point
	virtual bool doesCollide(Vector2 point);

	// test collision with each collider subclass
	virtual bool doesCollideWithAABox(AABox* box) = 0;
	virtual bool doesCollideWithOBox(OBox* box) = 0;
	virtual bool doesCollideWithCircle(CircleCollider* circle) = 0;

protected:
	Vector2 m_min;
	Vector2 m_max;
};

