#pragma once
#include "Collider.h"
#include <vector>

class CircleCollider :
	public Collider
{
public:
	CircleCollider();
	CircleCollider(Vector2 centre, float radius);
	~CircleCollider();

	// test collision with collider of unknown concrete type
	virtual bool doesCollide(Collider* other);

	// test collision with point
	virtual bool doesCollide(Vector2 point);

	// test collision with each collider subclass
	virtual bool doesCollideWithAABox(AABox* box);
	virtual bool doesCollideWithOBox(OBox* box);
	virtual bool doesCollideWithCircle(CircleCollider* circle);

	// Accessor and mutator functions
	Vector2 getCentre() {
		return m_centre;
	}
	void setCentre(Vector2 centre) {
		m_centre = centre;
	}

	float getRadius() {
		return m_radius;
	}

	void setRadius(float radius) {
		m_radius = radius;
	}

	// Fit CircleCollider around points given
	void fitPoints(std::vector<Vector2> points);

	// Fit CircleCollider around circles given
	void fitCircle(std::vector<CircleCollider> circles);

protected:
	Vector2 m_centre;
	float m_radius;
};

