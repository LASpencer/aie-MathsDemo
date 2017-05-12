#pragma once
#include <vector>
#include "Gizmos.h"
#include "Matrix.h"
#include "Vector.h"

using namespace lasmath;

// A 3D object within a hierarchical scene graph
class SceneObject3D
{
public:
	SceneObject3D();
	virtual ~SceneObject3D();

	// Adds a child to this SceneObject3D, or returns false if child can't be added
	bool addChild(SceneObject3D* child);

	// Accessor and Mutator methods
	void setLocalTransform(const Matrix4& transformation);
	Matrix4 getLocalTransform();
	Matrix4 getGlobalTransform();
	
	// Recalculates globalTransform and calls update on all children
	virtual void update(float deltaTime);
	// Performs any queued changes to array of children, and calls updateChildList on them
	void updateChildList();

	// Returns a vector of pointers to all descendents of this SceneObject3D
	std::vector<SceneObject3D*> getDescendants();

protected:
	Matrix4 m_globalTransform;					//Transformation matrix relative to global coordinates
	Matrix4 m_localTransform;					//Transformation matrix relative to parent
	SceneObject3D* m_parent;
	std::vector<SceneObject3D*> m_children;		
	std::vector<SceneObject3D*> m_toAdd;		//Vector of SceneObject3Ds to add next time it is safe to do so
	bool m_childrenLocked;						//Flag indicating that it is unsafe to modify vector of children

	// Calculates globalTransform by transforming localTransform by parent's globalTransform
	void calculateGlobalTransform();
};

