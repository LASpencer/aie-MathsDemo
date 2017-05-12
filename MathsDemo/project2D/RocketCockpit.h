#pragma once
#include "SceneObject3D.h"

// Sphere drawn at front of Rocket
class RocketCockpit :
	public SceneObject3D
{
public:
	RocketCockpit();
	~RocketCockpit();

	virtual void update(float deltaTime);


	static const Vector4 COLOUR;
	static const float RADIUS;
	static const size_t SEGMENTS;		// Number of rows and columsn of polygons to render
};

