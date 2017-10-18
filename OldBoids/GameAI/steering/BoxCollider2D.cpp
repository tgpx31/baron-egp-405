#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include <allegro5\allegro_primitives.h>
#include <cmath>

BoxCollider2D::BoxCollider2D(Vector2D position, float width, float height) : mPosition(position), mWidth(width), mHeight(height)
{
}

BoxCollider2D::~BoxCollider2D()
{
}

bool BoxCollider2D::checkCollision(BoxCollider2D * other)
{
	Vector2D oPosition = other->getPosition();
	float oWidth = other->getWidth();
	float oHeight = other->getHeight();

	if (mPosition.getX() < oPosition.getX() + mWidth &&
		mPosition.getX() + mWidth > oPosition.getX() &&
		mPosition.getY() < oPosition.getY() + oHeight &&
		mPosition.getY() + mHeight > oPosition.getY())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BoxCollider2D::checkCollision(CircleCollider2D* circle)
{
	float distX = std::abs(circle->getCenter().getX() - mPosition.getX() - (mWidth / 2));
	float distY = std::abs(circle->getCenter().getY() - mPosition.getY() - (mHeight / 2));

	if (distX > (mWidth / 2 + circle->getRadius()))
		return false;
	if (distY > (mHeight / 2 + circle->getRadius()))
		return false;

	if (distX <= (mWidth / 2))
		return true;
	if (distY <= (mHeight / 2))
		return true;

	float dx = distX - (mWidth / 2);
	float dy = distY - (mHeight / 2);

	return (std::pow(dx, 2) + std::pow(dy, 2) <= std::pow(circle->getRadius(), 2));
}

void BoxCollider2D::draw()
{
	al_draw_rectangle(mPosition.getX(), mPosition.getY(), mPosition.getX() + mWidth, mPosition.getY() + mHeight, al_map_rgb(0, 255, 0), 1.0f);
}
