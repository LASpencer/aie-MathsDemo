#pragma once
#include "SceneObject.h"
#include "ArmPart.h"
#include "RobotHand.h"
#include "Input.h"

class RobotArm :
	public SceneObject
{
public:
	RobotArm();
	RobotArm(Vector2 position);
	~RobotArm();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	static const int SHOULDER_LEFT_KEY;
	static const int SHOULDER_RIGHT_KEY;
	static const int ELBOW_LEFT_KEY;
	static const int ELBOW_RIGHT_KEY;
	static const int WRIST_LEFT_KEY;
	static const int WRIST_RIGHT_KEY;
	static const int GRIP_KEY;
	static const int MOVE_LEFT_KEY;
	static const int MOVE_RIGHT_KEY;
	static const float SHOULDER_TURN_RATE;
	static const float ELBOW_TURN_RATE;
	static const float WRIST_TURN_RATE;
	static const float MOVE_RATE;

protected:
	ArmPart*		m_shoulder;
	ArmPart*		m_elbow;
	RobotHand*		m_hand;

	void setupParts();
};

