//#include "Wall.h"
//#include <allegro5\allegro_color.h>
//#include <allegro5\allegro_primitives.h>
//#include <math.h>
//
//Wall::Wall(Vector2D topLeftPoint, Vector2D bottomRightPoint) : mTopLeftPoint(topLeftPoint), mBottomRightPoint(bottomRightPoint)
//{
//	mpBoxCollider = new BoxCollider2D(mTopLeftPoint, std::abs(mBottomRightPoint.getX() - mTopLeftPoint.getX()), std::abs(mBottomRightPoint.getY() - mTopLeftPoint.getY()));
//}
//
//Wall::~Wall()
//{
//	delete mpBoxCollider;
//}
//
//void Wall::draw()
//{
//	al_draw_filled_rectangle(mTopLeftPoint.getX(), mTopLeftPoint.getY(), mBottomRightPoint.getX(), mBottomRightPoint.getY(), al_map_rgb(0, 0, 0));
//}
