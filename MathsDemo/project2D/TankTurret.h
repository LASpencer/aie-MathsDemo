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
	static const int LEFT_AIM_KEY;
	static const int RIGHT_AIM_KEY;
	static const float AIM_RATE;

protected:
	aie::Texture* m_turretSprite;
};

