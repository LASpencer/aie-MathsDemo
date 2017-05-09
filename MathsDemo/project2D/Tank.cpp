#include "Tank.h"
#include "Input.h"
#include "Texture.h"
#include "TankTurret.h"
#include "Bullet.h"
#include "OBox.h"
#include "Wall.h"
#include "Obstacle.h"

const int Tank::LEFT_TURN_KEY = aie::INPUT_KEY_A;
const int Tank::RIGHT_TURN_KEY = aie::INPUT_KEY_D;
const int Tank::FORWARD_KEY = aie::INPUT_KEY_W;
const int Tank::REVERSE_KEY = aie::INPUT_KEY_S;
const int Tank::FIRE_KEY = aie::INPUT_KEY_SPACE;
const int Tank::LEFT_AIM_KEY = aie::INPUT_KEY_LEFT;
const int Tank::RIGHT_AIM_KEY = aie::INPUT_KEY_RIGHT;

const float Tank::AIM_RATE = 1.5f;
const float Tank::TURN_RATE = 1.0f;
const float Tank::FORWARD_SPEED = 75.0f;
const float Tank::REVERSE_SPEED = 50.0f;
const float Tank::BULLET_COOLDOWN = 0.5f;
const float Tank::BARREL_LENGTH = 50.0f + Bullet::RADIUS;

Tank::Tank() : m_tankSprite(new aie::Texture("./textures/tankGreen.png")), m_turret(new TankTurret()), m_bulletCD(0.0f)
{
	m_localTransform.setIdentity();
	addChild(m_turret);
}

Tank::Tank(Vector2 position) : m_tankSprite(new aie::Texture("./textures/tankGreen.png")), m_turret(new TankTurret()), m_bulletCD(0.0f)
{
	m_localTransform.setIdentity();
	m_localTransform[2] = { position[0],position[1],1 };
	calculateGlobalTransform();
	addChild(m_turret);
}


Tank::~Tank()
{
	delete m_tankSprite;
}

void Tank::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	bool leftTurn = input->isKeyDown(LEFT_TURN_KEY);
	bool rightTurn = input->isKeyDown(RIGHT_TURN_KEY);
	bool forward = input->isKeyDown(FORWARD_KEY);
	bool reverse = input->isKeyDown(REVERSE_KEY);
	bool leftAim = input->isKeyDown(LEFT_AIM_KEY);
	bool rightAim = input->isKeyDown(RIGHT_AIM_KEY);
	bool fire = input->isKeyDown(FIRE_KEY);

	// If one turn key pressed, rotate in that direction
	if (leftTurn && !rightTurn) {
		// rotate counterclockwise
		rotate(TURN_RATE*deltaTime);
	}
	else if (rightTurn && !leftTurn) {
		// rotate clockwise
		rotate(-TURN_RATE*deltaTime);
	}
	// TODO limited firing arc
	if (leftAim && !rightAim) {
		m_turret->rotate(AIM_RATE*deltaTime);
	}
	else if (rightAim && !leftAim) {
		m_turret->rotate(-AIM_RATE*deltaTime);
	}
	if (forward && !reverse) {
		// move forward
		translate({ 0,FORWARD_SPEED*deltaTime });
	}
	else if (reverse && !forward) {
		// move in reverse
		translate({ 0,-REVERSE_SPEED*deltaTime });
	}

	// decrease bullet cooldown timer
	if (m_bulletCD > 0) {
		m_bulletCD -= deltaTime;
	}

	SceneObject::update(deltaTime);

	// fire bullet after updating global transform
	if (fire) {
		fireBullet();
	}
}

void Tank::draw(aie::Renderer2D * renderer)
{
	// draw tank
	renderer->drawSpriteTransformed3x3(m_tankSprite, (float*)m_globalTransform);
	// draw children
	SceneObject::draw(renderer);
	
}

void Tank::fireBullet()
{
	if (m_bulletCD <= 0.0f) {
		Vector3 muzzlePosition = m_turret->getLocalTransform() * Vector3(0, BARREL_LENGTH, 1);
		Vector3 muzzleDirection = muzzlePosition;
		muzzleDirection[2] = 0.0f;
		// convert position and direction to parent's frame of reference
		muzzlePosition = m_localTransform * muzzlePosition;
		muzzleDirection = m_localTransform * muzzleDirection;
		muzzleDirection.normalise();
		Bullet* bullet = new Bullet((Vector2)muzzlePosition, (Vector2)muzzleDirection * Bullet::DEFAULT_SPEED);
		m_parent->addChild(bullet);
		m_bulletCD = BULLET_COOLDOWN;
	}
}

void Tank::notifyCollision(SceneObject * other, Vector2 penetration)
{
	//TODO if wall was hit, stop moving
	if (dynamic_cast<Wall*>(other) != nullptr) {
		globalTranslate(penetration);
	}
	else if (dynamic_cast<Obstacle*>(other) != nullptr) {
		//TODO if obstacle was hit, both pushed back half penetration
		globalTranslate(0.5*penetration);
	}
}

void Tank::notifyOutOfBounds(Vector2 penetration)
{
	// move back into bounds
	globalTranslate(penetration);
}

void Tank::setupCollider()
{
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	((OBox*)m_collider)->setHalfExtents((Vector2)m_globalTransform[0] * 37.5, (Vector2)m_globalTransform[1] * 35);
	((OBox*)m_collider)->setCentre((Vector2)m_globalTransform[2]);
}
