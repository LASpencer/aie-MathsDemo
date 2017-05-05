#include "RobotArm.h"
#include "Texture.h"

const float RobotArm::SHOULDER_TURN_RATE = 1.0f;
const float RobotArm::ELBOW_TURN_RATE = 1.0f;
const float RobotArm::WRIST_TURN_RATE = 1.0f;

RobotArm::RobotArm()
{
	m_localTransform.setIdentity();
	setupParts();
}

RobotArm::RobotArm(Vector2 position)
{
	m_localTransform.setIdentity();
	m_localTransform[2] = { position[0],position[1],1 };
	setupParts();
}


RobotArm::~RobotArm()
{
}

void RobotArm::update(float deltaTime)
{
	//TODO check for input to rotate arm components
	aie::Input* input = aie::Input::getInstance();
	bool shoulderLeft = input->isKeyDown(SHOULDER_LEFT_KEY);
	bool shoulderRight = input->isKeyDown(SHOULDER_RIGHT_KEY);
	bool elbowLeft = input->isKeyDown(ELBOW_LEFT_KEY);
	bool elbowRight = input->isKeyDown(ELBOW_RIGHT_KEY);
	bool wristLeft = input->isKeyDown(WRIST_LEFT_KEY);
	bool wristRight = input->isKeyDown(WRIST_RIGHT_KEY);
	if (shoulderLeft && !shoulderRight) {
		m_shoulder->rotate(SHOULDER_TURN_RATE*deltaTime);
	}
	else if (shoulderRight && !shoulderLeft) {
		m_shoulder->rotate(-SHOULDER_TURN_RATE*deltaTime);
	}
	if (elbowLeft && !elbowRight) {
		m_elbow->rotate(ELBOW_TURN_RATE*deltaTime);
	}
	else if (elbowRight && !elbowLeft) {
		m_elbow->rotate(-ELBOW_TURN_RATE*deltaTime);
	}
	if (wristLeft && !wristRight) {
		m_hand->rotate(WRIST_TURN_RATE*deltaTime);
	}
	else if (wristRight && !wristLeft) {
		m_hand->rotate(-WRIST_TURN_RATE*deltaTime);
	}
	SceneObject::update(deltaTime);
}

void RobotArm::draw(aie::Renderer2D * renderer)
{
	SceneObject::draw(renderer);
}

void RobotArm::setupParts()
{
	m_shoulder = new ArmPart(new aie::Texture("./textures/ArmShoulder.png"));
	m_shoulder->rotate(-0.5f*(3.14159f));

	m_elbow = new ArmPart(new aie::Texture("./textures/ArmElbow.png"));
	m_elbow->translate(Vector2(0,185));
	m_elbow->rotate(0.5f*(3.14159f));

	m_hand = new RobotHand(new aie::Texture("./textures/ArmHand.png"));
	m_hand->translate(Vector2(0, 185));

	addChild(m_shoulder);
	m_shoulder->addChild(m_elbow);
	m_elbow->addChild(m_hand);
}
