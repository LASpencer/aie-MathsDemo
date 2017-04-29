#pragma once
#include "Collider.h"
class CircleCollider :
	public Collider
{
public:
	CircleCollider();
	~CircleCollider();

	// test collision with collider of unknown concrete type
	virtual bool doesCollide(Collider* other);

	// test collision with point
	virtual bool doesCollide(Vector2 point);

	// test collision with each collider subclass
	virtual bool doesCollideWithAABox(AABox* box);
	virtual bool doesCollideWithOBox(OBox* box);
	virtual bool doesCollideWithCircle(CircleCollider* circle);

protected:
	Vector2 m_centre;
	float m_radius;
};

