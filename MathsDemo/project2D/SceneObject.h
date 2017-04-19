#pragma once
#include <vector>
#include "Renderer2D.h"
#include "Matrix.h"
#include "Vector.h"

using namespace lasmath;

class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();

	void addChild(SceneObject* child);

	//TODO remove child

	void setLocalTransform(const Matrix3& transformation);

	void rotate(float angle);
	void scale(const Vector2& proportions);
	void translate(const Vector2& vec);
	void transform(const Matrix3& transformation);

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

protected:
	Matrix3 m_globalTransform;
	Matrix3 m_localTransform;

	SceneObject* m_parent;
	std::vector<SceneObject*> m_children;

	void calculateGlobalTransform();
};

