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

	static const int SHOULDER_LEFT_KEY = aie::INPUT_KEY_A;
	static const int SHOULDER_RIGHT_KEY = aie::INPUT_KEY_D;
	static const int ELBOW_LEFT_KEY = aie::INPUT_KEY_Q;
	static const int ELBOW_RIGHT_KEY = aie::INPUT_KEY_E;
	static const int WRIST_LEFT_KEY = aie::INPUT_KEY_LEFT;
	static const int WRIST_RIGHT_KEY = aie::INPUT_KEY_RIGHT;
	static const float SHOULDER_TURN_RATE;
	static const float ELBOW_TURN_RATE;
	static const float WRIST_TURN_RATE;

protected:
	ArmPart*		m_shoulder;
	ArmPart*		m_elbow;
	RobotHand*		m_hand;

	void setupParts();
};

