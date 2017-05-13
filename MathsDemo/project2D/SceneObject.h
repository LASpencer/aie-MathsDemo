#pragma once
#include <vector>
#include "Renderer2D.h"
#include "Matrix.h"
#include "Vector.h"

using namespace lasmath;

class Collider;

// A 2D object within a hierarchical scene graph
class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();

	// Adds a child to this SceneObject, or returns false if child can't be added
	bool addChild(SceneObject* child);

	// Destroys a child of this SceneObject, or returns false if it's unsafe to destroy child
	bool removeChild(SceneObject* child);
	// Reparents child of this SceneObject to target, or returns false if it's unsafe to make transfer
	bool transferChild(SceneObject* child, SceneObject* target);

	// Accessor and Mutator methods
	void setLocalTransform(const Matrix3& transformation);
	Matrix3 getLocalTransform();
	Matrix3 getGlobalTransform();

	// Rotate the SceneObject by angle radians
	void rotate(float angle);
	// Scale SceneObject by given proportions
	void scale(const Vector2& proportions);
	// Translate SceneObject by given displacement, in terms of SceneObject's axes
	void translate(const Vector2& vec);
	// Apply transformaiton to SceneObject
	void transform(const Matrix3& transformation);

	// Translate SceneObject by displacement given in global frame of reference
	void globalTranslate(const Vector2& vec);

	// Recalculates globalTransform and calls update on all children
	virtual void update(float deltaTime);
	
	// Calls draw on all children 
	virtual void draw(aie::Renderer2D* renderer);

	// Returns true if this SceneObject can be safely removed from its parent
	bool safeToDelete();

	// Returns true if this SceneObject is flagged to be deleted
	bool markedForDeletion();

	// Returns true if this SceneObject can be safely transferred to another parent
	bool safeForTransfer();

	// Returns pointer to this SceneObject's parent, or nullptr if it is the root
	SceneObject* getParent();

	// Returns pointer to SceneObject at root of the scene graph
	SceneObject* getRoot();

	// Returns a vector of pointers to all descendents of this SceneObject
	std::vector<SceneObject*> getDescendants();

	// Informs SceneObject that it has collided with other sceneObject
	virtual void notifyCollision(SceneObject* other, Vector2 penetration);
	//	Tell SceneObject it is out of bounds
	virtual void notifyOutOfBounds(Vector2 penetration);

	// Returns pointer to this SceneObject's collider
	Collider* getCollider();

protected:
	Matrix3 m_globalTransform;					// Transformation matrix relative to global coordinates
	Matrix3 m_localTransform;					// Transformation matrix relative to parent

	SceneObject* m_parent;
	std::vector<SceneObject*> m_children;
	std::vector<SceneObject*> m_toAdd;			// Vector of SceneObjects to add next time it is safe to do so
	std::vector<SceneObject*> m_toDelete;		// Vector of children to destroy next time it is safe to do so
	std::vector < std::pair<SceneObject*, SceneObject*>> m_toTransfer;		// Vector containing child and SceneObject to transfer it to next time it is safe to do so
	bool m_childrenLocked;						// Flag indicating it is unsafe to modify vector of children
	bool m_deletionFlag;						// Flag indicating this SceneObject is queued to be deleted
	bool m_transferFlag;						// Flag indicating this SceneObject is queued to be transfered to a new parent
	bool m_transferTargetFlag;					// Flag indicating this SceneObject is queued to have a child transfered to it

	Collider* m_collider;						// Collider used for this SceneObject

	// Calculates globalTransform by transforming localTransform by parent's globalTransform
	void calculateGlobalTransform();

	// Creates the collider for this SceneObject
	virtual void setupCollider();

	// Performs any queued changes to array of children, and to descendent's children
	void updateChildList();

	// Change any variables necessary for transfer to new parent
	virtual void prepareForTransfer(SceneObject* target);

	// Remove child from children, and add as child to target
	void performChildTransfer(SceneObject* child, SceneObject* target);

};

