#pragma once

#include "Trackable.h"
#include "Vector2D.h"

class CircleCollider2D;

class BoxCollider2D : public Trackable
{
private:
	Vector2D mPosition;
	float mWidth;
	float mHeight;
public:
	BoxCollider2D(Vector2D position, float width, float height);
	~BoxCollider2D();

	inline Vector2D getPosition() { return mPosition; };
	inline float getWidth() { return mWidth; };
	inline float getHeight() { return mHeight; };
	inline void setPosition(Vector2D position) { mPosition = position; };
	inline void setWidth(float width) { mWidth = width; };
	inline void setHeight(float height) { mHeight = height; };

	bool checkCollision(BoxCollider2D* other);
	bool checkCollision(CircleCollider2D* circle);
	
	void draw();
};