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

lasmath::Matrix4 GLMAdaptor::lookAt(const lasmath::Vector3 & eye, const lasmath::Vector3 & centre, const lasmath::Vector3 & up)
{
	lasmath::Matrix4 rotationMatrix, translationMatrix, transform, lookatMatrix;
	// Move to eye
	lasmath::Vector3 translation =  eye;
	//zAxis is normal vector pointing from centre to eye
	lasmath::Vector3 zAxis = eye-centre;
	bool zValid = zAxis.normalise();
	if(!zValid){
		//If eye at centre, set z axis to [0,0,1]
		zAxis = { 0,0,1 };
	}
	//xAxis is normal vector perpendicular to zAxis and up
	lasmath::Vector3 xAxis = up.cross(zAxis);
	bool xValid = xAxis.normalise();
	if(!xValid) {
		// If up and zAxis parallel, set xAxis to normal vector orthogonal to zAxis
		xAxis = { zAxis[2],zAxis[2],-(zAxis[0]+zAxis[1]) };
		xValid = xAxis.normalise();
		if (!xValid) {
			//if zAxis[0] == -zAxis[1] and zAxis[2]==0, pick this vector instead
			xAxis = { -(zAxis[1]+zAxis[2]),zAxis[0],zAxis[0]};
			xAxis.normalise();
		}
	}
	//yAxis is normal vector perpendicular to xAxis and zAxis
	lasmath::Vector3 yAxis = zAxis.cross(xAxis);
	zAxis.normalise();
	// Set up matrices
	translationMatrix.setIdentity();
	translationMatrix[3] += (lasmath::Vector4)translation;
	rotationMatrix.setIdentity();
	rotationMatrix[0] = (lasmath::Vector4)xAxis;
	rotationMatrix[1] = (lasmath::Vector4)yAxis;
	rotationMatrix[2] = (lasmath::Vector4)zAxis;
	transform = translationMatrix * rotationMatrix;
	// Invert transformation to get lookatMatrix
	transform.calculateInverse(lookatMatrix);
	return lookatMatrix;
}

lasmath::Matrix4 GLMAdaptor::perspective(float fovy, float aspect, float near, float far)
{
	float invHalfTan = 1.0f / tanf(fovy*0.5f);
	float invDepth = 1.0f / (near - far);
	lasmath::Matrix4 projection = lasmath::Matrix4(invHalfTan / aspect, 0, 0, 0,
													0, invHalfTan, 0, 0,
													0, 0, (near + far) * invDepth, -1,
													0, 0, 2 * near*far * invDepth, 0);
	return projection;
}
