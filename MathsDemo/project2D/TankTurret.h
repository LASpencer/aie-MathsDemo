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

protected:
	aie::Texture* m_turretSprite;
};

