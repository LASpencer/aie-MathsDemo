#pragma once
#include <vector>
#include "Renderer2D.h"
#include "Matrix.h"
#include "Vector.h"

using namespace lasmath;

class Collider;

class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();

	bool addChild(SceneObject* child);

	bool removeChild(SceneObject* child);

	bool transferChild(SceneObject* child, SceneObject* target);

	void setLocalTransform(const Matrix3& transformation);
	Matrix3 getLocalTransform();
	Matrix3 getGlobalTransform();

	void rotate(float angle);
	void scale(const Vector2& proportions);
	void translate(const Vector2& vec);
	void transform(const Matrix3& transformation);

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	void updateChildList();

	bool safeToDelete();

	bool markedForDeletion();

	bool safeForTransfer();

	std::vector<SceneObject*> getDescendants();

	virtual void notifyCollision(SceneObject* other, Vector2 penetration);
	//	Tell SceneObject it is out of bounds
	virtual void notifyOutOfBounds(Vector2 penetration);

	Collider* getCollider();

protected:
	Matrix3 m_globalTransform;
	Matrix3 m_localTransform;

	SceneObject* m_parent;
	std::vector<SceneObject*> m_children;
	std::vector<SceneObject*> m_toAdd;
	std::vector<SceneObject*> m_toDelete;
	std::vector < std::pair<SceneObject*, SceneObject*>> m_toTransfer;
	bool m_childrenLocked;
	bool m_deletionFlag;
	bool m_transferFlag;
	bool m_transferTargetFlag;

	Collider* m_collider;

	void calculateGlobalTransform();
	virtual void setupCollider();

	// Change any variables necessary for transfer to new parent
	virtual void prepareForTransfer(SceneObject* target);

	// Remove child from children, and add as child to target
	void performChildTransfer(SceneObject* child, SceneObject* target);

};

