#pragma once
#include "SceneObject.h"

class TankTurret;

class Tank :
	public SceneObject
{
public:
	Tank();
	Tank(Vector2 position);
	~Tank();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	static const int LEFT_TURN_KEY;
	static const int RIGHT_TURN_KEY;
	static const int FORWARD_KEY;
	static const int REVERSE_KEY;
	static const float TURN_RATE;
	static const float FORWARD_SPEED;
	static const float REVERSE_SPEED;

protected:
	TankTurret* m_turret;
	aie::Texture* m_tankSprite;
};

