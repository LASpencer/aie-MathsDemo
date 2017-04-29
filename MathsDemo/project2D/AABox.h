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

	// test collision with AABox
	virtual bool doesCollideWithAABox(AABox* box);
	virtual bool doesCollideWithOBox(OBox* box);
	virtual bool doesCollideWithCircle(CircleCollider* circle);

protected:
	Vector2 m_min;
	Vector2 m_max;
};

