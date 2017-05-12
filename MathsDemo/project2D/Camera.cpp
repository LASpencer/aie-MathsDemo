#include "Camera.h"
#include "GLMAdaptor.h"

const float Camera::MIN_DISTANCE = 0.1f;
const float Camera::MAX_DISTANCE = 1000;
const float Camera::DEF_DISTANCE = 10;
const float Camera::MAX_TILT = 3.1415926f*0.4f;
const float Camera::ZOOM_RATE = 0.5f;
const float Camera::PAN_RATE = 0.5f;
const float Camera::TILT_RATE = 0.5f;

Camera::Camera() : m_distance(DEF_DISTANCE), m_pan(0),m_tilt(0)
{
}

Camera::Camera(float distance, float pan, float tilt):m_pan(pan)
{
	// Clamp distance between MIN_DISTANCE and MAX_DISTANCE
	m_distance = std::min(MAX_DISTANCE, std::max(MIN_DISTANCE, distance));
	// Clamp tilt within +/- MAX_TILT
	m_tilt = std::min(MAX_TILT, std::max(-MAX_TILT, tilt));
}


Camera::~Camera()
{
}

void Camera::update(float deltaTime)
{
	Matrix4 rotate, tilt, position;
	// Rotate camera to correct orientation
	rotate.setRotateZ(m_pan);
	tilt.setRotateY(m_tilt);
	// Move camera back to set distance
	position.setIdentity();
	position[3][0] = m_distance;
	m_localTransform = rotate * tilt* position;
	SceneObject3D::update(deltaTime);
}

Matrix4 Camera::lookAt()
{
	Vector3 eye, centre, up;
	// Eye is camera's position
	eye = (Vector3)m_globalTransform[3];
	// centre is parent's position, or origin if no parent
	if (m_parent != nullptr) {
		centre = (Vector3)m_parent->getGlobalTransform()[3];
	}
	else {
		centre = { 0,0,0 };
	}
	// Camera's Z Axis used as up
	up = (Vector3)m_globalTransform[2];
	return GLMAdaptor::lookAt(eye,centre,up);
}

void Camera::zoom(float proportion)
{
	m_distance += proportion*m_distance;
	// Clamp m_distance between MIN_DISTANCE and MAX_DISTANCE
	m_distance = std::min(MAX_DISTANCE, std::max(MIN_DISTANCE, m_distance));
}

void Camera::pan(float angle)
{
	m_pan += angle;
}

void Camera::tilt(float angle)
{
	m_tilt += angle;
	// Camp m_tilt within +/= MAX_TILT
	m_tilt = std::min(MAX_TILT, std::max(-MAX_TILT, m_tilt));
}
