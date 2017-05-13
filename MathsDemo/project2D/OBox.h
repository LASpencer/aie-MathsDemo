#pragma once
#include <vector>
#include "Collider.h"
#include "Matrix.h"


// An Oriented Bounding Box for collision testing
class OBox :
	public Collider
{
public:
	OBox();
	OBox(Vector2 xExtent, Vector2 yExtent, Vector2 centre);
	~OBox();

	// test collision with Collider of unknown concrete type
	virtual std::pair<bool, Vector2> doesCollide(Collider* other);

	// test collision with point
	virtual std::pair<bool, Vector2> doesCollide(Vector2 point);

	// test collision with plane
	virtual std::pair<bool, Vector2> doesCollide(Plane plane);

	// test collision with each collider subclass
	virtual std::pair<bool, Vector2> doesCollideWithAABox(AABox* box);
	virtual std::pair<bool, Vector2> doesCollideWithOBox(OBox* box);
	virtual std::pair<bool, Vector2> doesCollideWithCircle(CircleCollider* circle);

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

	// Returns all four corners, starting with centre+xExtent+yExtent
	std::tuple<Vector2, Vector2, Vector2, Vector2> getCorners();

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

	// Fit OBox around points given
	void fitPoints(std::vector<Vector2> points);


	// Calculates transformation matrix that would make this an AABox with corners (-1,-1) and (1,1)
	Matrix3 getInverseTransform();

protected:
	Vector2 m_xExtent;
	Vector2 m_yExtent;
	Vector2 m_centre;
};

