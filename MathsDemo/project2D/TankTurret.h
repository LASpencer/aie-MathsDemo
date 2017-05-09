#pragma once
#include "SceneObject.h"
class TankTurret :
	public SceneObject
{
public:
	TankTurret();
	~TankTurret();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	// Passes collision on to parent
	virtual void notifyCollision(SceneObject* other, Vector2 penetration);

	// Passes collision on to parent
	virtual void notifyOutOfBounds(Vector2 penetration);

protected:
	aie::Texture* m_turretSprite;

	virtual void setupCollider();
};

