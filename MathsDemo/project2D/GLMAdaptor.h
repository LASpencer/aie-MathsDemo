#pragma once
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Matrix.h"
#include "Vector.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class GLMAdaptor
{
public:
	GLMAdaptor();
	~GLMAdaptor();

	static vec3 Vector3Converter(const lasmath::Vector3& vector);
	static vec4 Vector4Converter(const lasmath::Vector4& vector);

	static mat4 Matrix4Converter(const lasmath::Matrix4& matrix);

	static lasmath::Matrix4 LookAt(const lasmath::Vector3& eye, const lasmath::Vector3& centre, const lasmath::Vector3& up);
	static lasmath::Matrix4 Perspective(float fovy, float aspect, float near, float far);
};

