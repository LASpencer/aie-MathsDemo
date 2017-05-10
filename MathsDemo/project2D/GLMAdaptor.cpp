#include "GLMAdaptor.h"



GLMAdaptor::GLMAdaptor()
{
}


GLMAdaptor::~GLMAdaptor()
{
}

vec3 GLMAdaptor::Vector3Converter(const lasmath::Vector3 & vector)
{
	return vec3(vector[0],vector[1],vector[2]);
}

vec4 GLMAdaptor::Vector4Converter(const lasmath::Vector4 & vector)
{
	return vec4(vector[0],vector[1],vector[2],vector[3]);
}

mat4 GLMAdaptor::Matrix4Converter(const lasmath::Matrix4 & matrix)
{
	return mat4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
			matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
			matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
			matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
}
