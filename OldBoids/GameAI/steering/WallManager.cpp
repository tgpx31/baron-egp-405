//#include "WallManager.h"
//
//WallManager::WallManager()
//{
//}
//
//WallManager::~WallManager()
//{
//	for (int i = 0; i < mWalls.size(); i++)
//	{
//		delete mWalls[i];
//		mWalls[i] = NULL;
//	}
//
//	mWalls.clear();
//}
//
//void WallManager::addWall(Wall * wall)
//{
//	mWalls.push_back(wall);
//}
//
//void WallManager::createWall(Vector2D topLeftPoint, Vector2D bottomRightPoint)
//{
//	mWalls.push_back(new Wall(topLeftPoint, bottomRightPoint));
//}
//
//bool WallManager::checkCollision(KinematicUnit* other)
//{
//	for (int i = 0; i < mWalls.size(); i++)
//	{
//		if (mWalls[i]->getBoxCollider()->checkCollision(other->getCircleCollider()))
//			return true;
//	}
//	
//	return false;
//}
//
//void WallManager::draw(bool drawCollider)
//{
//	for (int i = 0; i < mWalls.size(); i++)
//	{
//		mWalls[i]->draw();
//
//		if (drawCollider)
//			mWalls[i]->getBoxCollider()->draw();
//	}
//}
