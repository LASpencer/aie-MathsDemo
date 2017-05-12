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
	SceneObject::update(deltaTime);
}

void Wall::draw(aie::Renderer2D * renderer)
{
	//Get start and end points of wall
	Vector3 start = (Vector3)m_start;
	start[2] = 1;
	Vector3 end = (Vector3)m_end;
	end[2] = 1;
	// Transform start and end points to global coordinates
	start = m_globalTransform*start;
	end = m_globalTransform*end;
	//HACK figure out how to do width of line
	// Draw line from start to end point
	renderer->setRenderColour(0xFF2000FF);
	renderer->drawLine(start[0], start[1], end[0], end[1], WIDTH);
	SceneObject::draw(renderer);
}


void Wall::setupCollider()
{
	//TODO
	if (m_collider == nullptr) {
		m_collider = new OBox();
	}
	//HACK write less expensive method (calculate for local once, then transform to global?)
	// Get start and end points of wall
	Vector3 start = (Vector3)m_start;
	start[2] = 1;
	Vector3 end = (Vector3)m_end;
	end[2] = 1;
	// Transform to global coordinates
	start = m_globalTransform*start;
	end = m_globalTransform*end;
	// Centre of OBox is midway between start and end
	Vector3 centre = 0.5f * (start + end);
	// yExtent is vector from end to centre
	Vector2 yExtent = (Vector2)(end - centre);
	// xExtent is orthogonal to yExtent with magnitude of half width
	Vector2 xExtent = { yExtent[1],-yExtent[0] };
	xExtent.normalise();
	xExtent = xExtent * WIDTH*0.5f;
	// Draw OBox
	((OBox*)m_collider)->setCentre((Vector2)centre);
	((OBox*)m_collider)->setHalfExtents(xExtent, yExtent);
}
