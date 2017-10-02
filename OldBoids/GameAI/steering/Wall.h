#pragma once

#include "Trackable.h"
#include "Vector2D.h"
#include "BoxCollider2D.h"

class Wall : public Trackable
{
private:
	Vector2D mTopLeftPoint;
	Vector2D mBottomRightPoint;
	BoxCollider2D* mpBoxCollider;
	float mThickness;

public:
	Wall(Vector2D topLeftPoint, Vector2D bottomRightPoint);
	~Wall();

	inline BoxCollider2D* getBoxCollider() { return mpBoxCollider; };

	void draw();
};