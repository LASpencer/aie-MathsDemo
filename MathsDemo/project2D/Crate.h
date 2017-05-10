#pragma once
#include "SceneObject.h"
class Crate :
	public SceneObject
{
public:
	Crate();
	Crate(Vector2 position, const char* sprite = DEFAULT_SPRITE);
	~Crate();

	virtual void update(float deltaTime);

	virtual void draw(aie::Renderer2D* renderer);

	virtual void notifyCollision(SceneObject* other, Vector2 penetration);

	virtual void notifyOutOfBounds(Vector2 penetration);

	bool isHeld() {
		return m_held;
	}

	void setHeld(bool held) {
		m_held = held;
	}

	static const char* DEFAULT_SPRITE;

protected:
	aie::Texture* m_sprite;
	float m_width, m_height;
	bool m_held;

	virtual void setupCollider();
};

