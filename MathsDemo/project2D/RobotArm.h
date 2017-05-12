#pragma once
#include "SceneObject.h"
#include "ArmPart.h"
#include "RobotHand.h"
#include "Input.h"

// An object for positioning and controlling the ArmParts and RobotHand
class RobotArm :
	public SceneObject
{
public:
	RobotArm();
	RobotArm(Vector2 position);
	~RobotArm();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	// Input keys controlling RobotArm
	static const int SHOULDER_LEFT_KEY;	
	static const int SHOULDER_RIGHT_KEY;
	static const int ELBOW_LEFT_KEY;
	static const int ELBOW_RIGHT_KEY;
	static const int WRIST_LEFT_KEY;
	static const int WRIST_RIGHT_KEY;
	static const int GRIP_KEY;
	static const int MOVE_LEFT_KEY;
	static const int MOVE_RIGHT_KEY;
	
	static const float SHOULDER_TURN_RATE;		// Radians per second
	static const float ELBOW_TURN_RATE;			// Radians per second
	static const float WRIST_TURN_RATE;			// Radians per second
	static const float MOVE_RATE;

protected:
	ArmPart*		m_shoulder;		// Arm part attached to RobotArm
	ArmPart*		m_elbow;		// Arm part attached to end of m_shoulder
	RobotHand*		m_hand;			// Hand attached to end of m_elbow

	// Set up ArmParts and RobotHand in correct positions and heirarchy
	void setupParts();
};

