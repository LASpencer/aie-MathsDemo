#include "Wall.h"
#include "OBox.h"

const float Wall::WIDTH = 5.0f;

Wall::Wall(): m_start(), m_end()
{
}

Wall::Wall(Vector2 start, Vector2 end): m_start(start), m_end(end)
{
}


Wall::~Wall()
{
}

void Wall::update(float deltaTime)
{
	//TODO
	SceneObject::update(deltaTime);
}

void Wall::draw(aie::Renderer2D * renderer)
{
	//TODO
	Vector3 start = (Vector3)m_start;
	start[2] = 1;
	Vector3 end = (Vector3)m_end;
	end[2] = 1;
	start = m_globalTransform*start;
	end = m_globalTransform*end;
	//HACK figure out how to do width of line
	renderer->setRenderColour(0xFF2000FF);
	renderer->drawLine(start[0], start[1], end[0], end[1], WIDTH);
	SceneObject::draw(renderer);
}

void Wall::notifyCollision(SceneObject * other)
{
	//TODO
}

void Wall::setupCollider()
{
	//TODO
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	//HACK write less expensive method (calculate for local once, then transform to global?)
	Vector3 start = (Vector3)m_start;
	start[2] = 1;
	Vector3 end = (Vector3)m_end;
	end[2] = 1;
	start = m_globalTransform*start;
	end = m_globalTransform*end;
	Vector3 centre = 0.5f * (start + end);
	Vector2 yExtent = (Vector2)(end - centre);
	Vector2 xExtent = { yExtent[1],-yExtent[0] };
	xExtent.normalise();
	xExtent = xExtent * WIDTH*0.5f;
	((OBox*)m_collider)->setCentre((Vector2)centre);
	((OBox*)m_collider)->setHalfExtents(xExtent, yExtent);
}
