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

	// test collision with Ray
	virtual bool isHitByRay(Ray* ray);

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
		//TODO throw exception if invalid vectors
		m_xExtent = xExtent;
		m_yExtent = yExtent;
	}

	void setCentre(Vector2 centre) {
		m_centre = centre;
	}

	// Sets half extents and centre given by 3x3 matrix
	void setBoxMatrix(Matrix3 Orientation) {
		setHalfExtents((Vector2)Orientation[0], (Vector2)Orientation[1]);
		setCentre((Vector2)Orientation[2]);
	}

	// Transform OBox by transformation matrix
	void transform(Matrix3 matrix);

	// Fit OBox around points given
	void fitPoints(std::vector<Vector2> points);

	// Calculates transformation matrix that would make this an AABox with corners (-1,-1) and (1,1)
	Matrix3 getInverseTransform();

protected:
	Vector2 m_xExtent;
	Vector2 m_yExtent;
	Vector2 m_centre;
};

