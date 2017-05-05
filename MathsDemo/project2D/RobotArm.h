#pragma once
#include "SceneObject.h"
#include "ArmPart.h"
#include "RobotHand.h"

class RobotArm :
	public SceneObject
{
public:
	RobotArm();
	RobotArm(Vector2 position);
	~RobotArm();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

protected:
	ArmPart*		m_shoulder;
	ArmPart*		m_elbow;
	RobotHand*		m_hand;

	void setupParts();
};

