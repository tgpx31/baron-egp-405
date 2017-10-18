#pragma once

#include "Trackable.h"
#include "Vector2D.h"

class BoxCollider2D;

class CircleCollider2D : public Trackable
{
private:
	Vector2D mCenter;
	float mRadius;

public:
	CircleCollider2D(Vector2D center, float radius);
	~CircleCollider2D();

	inline Vector2D getCenter() { return mCenter; };
	inline float getRadius() { return mRadius; };
	inline void setCenter(Vector2D center) { mCenter = center; };
	inline void setRadius(float radius) { mRadius = radius; };

	bool checkCollision(CircleCollider2D* other);
	bool checkCollision(BoxCollider2D* other);
	
	void draw();
};
