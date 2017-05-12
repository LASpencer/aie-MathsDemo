#pragma once
#include "SceneObject3D.h"

// A SceneObject3D that can calculate the view matrix to its parent
class Camera :
	public SceneObject3D
{
public:
	Camera();
	Camera(float distance, float pan, float tilt);
	~Camera();

	virtual void update(float deltaTime);

	// Calculate the view matrix from this object looking at its parent, with the Z axis as up
	Matrix4 lookAt();

	void zoom(float proportion);
	void pan(float angle);
	void tilt(float angle);

	static const float MIN_DISTANCE;		//Minimum distance to parent
	static const float MAX_DISTANCE;		//Maximum distance from parent
	static const float DEF_DISTANCE;		//Default distance to parent
	static const float MAX_TILT;			//Maximum angle the camera can tilt up or down
	static const float ZOOM_RATE;			//Proportion of distance the camera moves per second
	static const float PAN_RATE;			//Rate at which camera pans around in radians per second
	static const float TILT_RATE;			//Rate at which camera tilts up or down in radians per second

protected:
	float m_distance;						// Distance to parent
	float m_pan;							// Angle from parent's X axis
	float m_tilt;							// Angle from plane containing parent's X and Y axes
};

