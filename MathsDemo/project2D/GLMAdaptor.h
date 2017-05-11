#pragma once
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Matrix.h"
#include "Vector.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

// Class containing methods necessary to control Gizmos with lasmath vectors and matrices
class GLMAdaptor
{
public:
	GLMAdaptor();
	~GLMAdaptor();

	// create glm::vec3 from lasmath::Vector3
	static vec3 Vector3Converter(const lasmath::Vector3& vector);

	// create glm::vec4 from lasmath::Vector4
	static vec4 Vector4Converter(const lasmath::Vector4& vector);

	// create glm::mat4 from lasmath::Matrix4
	static mat4 Matrix4Converter(const lasmath::Matrix4& matrix);

	// Calculate view matrix using lasmath classes
	static lasmath::Matrix4 lookAt(const lasmath::Vector3& eye, const lasmath::Vector3& centre, const lasmath::Vector3& up);
	
	// Calculate projection matric using lasmath classes
	static lasmath::Matrix4 perspective(float fovy, float aspect, float near, float far);
};

