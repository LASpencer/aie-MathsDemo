#pragma once
#include <vector>
#include "Collider.h"
#include "Matrix.h"

class OBox :
	public Collider
{
public:
	OBox();
	OBox(Vector2 xExtent, Vector2 yExtent, Vector2 centre);
	~OBox();

	// test collision with Collider of unknown concrete type
	virtual bool doesCollide(Collider* other);

	// test collision with point
	virtual bool doesCollide(Vector2 point);

	// test collision with each collider subclass
	virtual bool doesCollideWithAABox(AABox* box);
	virtual bool doesCollideWithOBox(OBox* box);
	virtual bool doesCollideWithCircle(CircleCollider* circle);

	// Accessor and mutator functions
	Vector2 getXExtent() {
		return m_xExtent;
	}
	Vector2 getYExtent() {
		return m_yExtent;
	}
	Vector2 getCentre() {
		return m_centre;
	}

	// Gets extents and centre as a 3x3 transformation matrix
	Matrix3 getBoxMatrix();

	void setHalfExtents(Vector2 xExtent, Vector2 yExtent) {
		m_xExtent = xExtent;
		m_yExtent = yExtent;
	}

	void setCentre(Vector2 centre) {
		m_centre = centre;
	}

	// Transform OBox by transformation matrix
	void transform(Matrix3 matrix);

	// Fit OBox around points given
	void fitPoints(std::vector<Vector2> points);

protected:
	Vector2 m_xExtent;
	Vector2 m_yExtent;
	Vector2 m_centre;
};

