#include "Camera.h"
#include "GLMAdaptor.h"

const float Camera::MIN_DISTANCE = 0.1f;
const float Camera::MAX_DISTANCE = 1000;
const float Camera::DEF_DISTANCE = 10;
const float Camera::MAX_TILT = 3.1415926f*0.3f;
const float Camera::ZOOM_RATE = 0.5f;
const float Camera::PAN_RATE = 0.5f;
const float Camera::TILT_RATE = 0.5f;

Camera::Camera() : m_distance(DEF_DISTANCE), m_pan(0),m_tilt(0)
{
}

Camera::Camera(float distance, float pan, float tilt):m_pan(pan)
{
	m_distance = std::min(MAX_DISTANCE, std::max(MIN_DISTANCE, distance));
	m_tilt = std::min(MAX_TILT, std::max(-MAX_TILT, tilt));
}


Camera::~Camera()
{
}

void Camera::update(float deltaTime)
{
	Matrix4 rotate, position;
	rotate.setEulerRotate(m_pan - 3.1415926f*0.25f, m_tilt, 3.1415926f*0.25f);
	position.setIdentity();
	position[3][0] = m_distance;
	position = rotate * position;
	m_localTransform[3] = position[3];
	SceneObject3D::update(deltaTime);
}

Matrix4 Camera::lookAt()
{
	Vector3 eye, centre, up;
	eye = (Vector3)m_globalTransform[3];
	if (m_parent != nullptr) {
		centre = (Vector3)m_parent->getGlobalTransform()[3];
	}
	else {
		centre = { 0,0,0 };
	}
	up = (Vector3)m_globalTransform[2];
	return GLMAdaptor::LookAt(eye,centre,up);
}

void Camera::zoom(float proportion)
{
	m_distance += proportion*m_distance;
	m_distance = std::min(MAX_DISTANCE, std::max(MIN_DISTANCE, m_distance));
}

void Camera::pan(float angle)
{
	m_pan += angle;
}

void Camera::tilt(float angle)
{
	m_tilt += angle;
	m_tilt = std::min(MAX_TILT, std::max(-MAX_TILT, m_tilt));
}
