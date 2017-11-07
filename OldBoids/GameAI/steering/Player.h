#ifndef PLAYER_H
#define PLAYER_H

#include "Vector2D.h"

class Sprite;
class GraphicsBuffer;

class Player
{
public:
	Player(Sprite *pSprite, const Vector2D& position, float orientation, float scale);
	~Player();

	void update(float dt);
	void draw(GraphicsBuffer *pBuffer);

	inline void setXVelocity(float xVel) { mVel.setX(xVel); };
	inline void setYVelocity(float yVel) { mVel.setY(yVel); };
	inline void setVelocity(float xVel, float yVel) { mVel.setX(xVel); mVel.setY(yVel); };
	inline void setVelocity(Vector2D vel) { mVel = vel; };

	inline float getYVel() { return mVel.getY(); };

	inline Vector2D getPosition() { return mPosition; };
	inline void setPosition(float x, float y) { mPosition = Vector2D(x, y); };

	inline float LerpFloat(float p0, float p1, float dt) { return p0 + ((p1 - p0) * dt); };
	inline Vector2D LerpVector2D(Vector2D startVec, const Vector2D &targetVec, float dt) 
	{
		Vector2D resultVec = Vector2D(LerpFloat(startVec.getX(),
		targetVec.getX(), dt), LerpFloat(startVec.getY(), targetVec.getY(), dt));
		return resultVec;
	};

	inline int isJumping() { return mJumping; };
	inline void setJumping(int val) { mJumping = val; };

	inline void setID(int id) { mID = id; };
	inline int getID() { return mID; };

	inline void setSprite(Sprite* spr) { mpSprite = spr; };

	inline void setOrientation(float or) { mOrientation = or ; };
	inline void setScale(float scale) { mLocalScale = scale ; };

	inline float getOrientation() { return mOrientation; };
	inline float getScale() { return mLocalScale; };

private:
	Sprite* mpSprite;

	Vector2D mPosition;
	float mOrientation;
	float mLocalScale;

	Vector2D mVel;
	Vector2D mTarget;

	int mJumping;
	int mID;
};

#endif