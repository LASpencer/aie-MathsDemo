#include "RobotArm.h"
#include "Texture.h"


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
