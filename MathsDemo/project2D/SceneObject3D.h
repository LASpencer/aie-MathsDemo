#pragma once
#include <vector>
#include "Gizmos.h"
#include "Matrix.h"
#include "Vector.h"

using namespace lasmath;

class SceneObject3D
{
public:
	SceneObject3D();
	virtual ~SceneObject3D();

	bool addChild(SceneObject3D* child);

	void setLocalTransform(const Matrix4& transformation);
	Matrix4 getLocalTransform();
	Matrix4 getGlobalTransform();

	virtual void update(float deltaTime);

	void updateChildList();

	std::vector<SceneObject3D*> getDescendants();

protected:
	Matrix4 m_globalTransform;
	Matrix4 m_localTransform;
	SceneObject3D* m_parent;
	std::vector<SceneObject3D*> m_children;
	std::vector<SceneObject3D*> m_toAdd;
	bool m_childrenLocked;

	void calculateGlobalTransform();
};

