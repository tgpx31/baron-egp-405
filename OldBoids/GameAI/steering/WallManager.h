#pragma once

#include "Trackable.h"
#include "Wall.h"
#include "KinematicUnit.h"
#include <vector>

class WallManager : public Trackable
{
private:
	std::vector<Wall*> mWalls;

public:
	WallManager();
	~WallManager();

	void addWall(Wall* wall);
	void createWall(Vector2D topLeftPoint, Vector2D bottomRightPoint);
	bool checkCollision(KinematicUnit* other);
	void draw(bool drawCollider);
};