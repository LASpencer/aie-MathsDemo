#pragma once
#include "SceneObject3D.h"
class Camera :
	public SceneObject3D
{
public:
	Camera();
	Camera(float distance, float pan, float tilt);
	~Camera();

	virtual void update(float deltaTime);
	Matrix4 lookAt();

	void zoom(float proportion);
	void pan(float angle);
	void tilt(float angle);

	static const float MIN_DISTANCE;
	static const float MAX_DISTANCE;
	static const float DEF_DISTANCE;
	static const float MAX_TILT;
	static const float ZOOM_RATE;
	static const float PAN_RATE;
	static const float TILT_RATE;

protected:
	float m_distance;
	float m_pan;
	float m_tilt;
};

