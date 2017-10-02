#include "CircleCollider2D.h"
#include "BoxCollider2D.h"
#include <allegro5\allegro_primitives.h>
#include <cmath>

CircleCollider2D::CircleCollider2D(Vector2D center, float radius)
{
	mCenter = center;
	mRadius = radius;
}

CircleCollider2D::~CircleCollider2D()
{
}

bool CircleCollider2D::checkCollision(CircleCollider2D* other)
{
	//(x2 - x1)^2 + (y2 - y1)^2 <= (r1+r2)^2
	return (std::pow((other->getCenter().getX() - mCenter.getX()), 2) + std::pow((other->getCenter().getY() - mCenter.getY()), 2) <= std::pow((other->getRadius() + mRadius), 2));
}

bool CircleCollider2D::checkCollision(BoxCollider2D* box)
{
	float distX = std::abs(mCenter.getX() - box->getPosition().getX() - (box->getWidth() / 2));
	float distY = std::abs(mCenter.getY() - box->getPosition().getY() - (box->getHeight() / 2));

	if (distX > (box->getWidth() / 2 + mRadius))
		return false;
	if (distY > (box->getHeight() / 2 + mRadius))
		return false;

	if (distX <= (box->getWidth() / 2))
		return true;
	if (distY <= (box->getHeight() / 2))
		return true;

	float dx = distX - (box->getWidth() / 2);
	float dy = distY - (box->getHeight() / 2);

	return (std::pow(dx, 2) + std::pow(dy, 2) <= std::pow(mRadius, 2));
}

void CircleCollider2D::draw()
{
	al_draw_circle(mCenter.getX(), mCenter.getY(), mRadius, al_map_rgb(0, 255, 0), 1.0f);
}
