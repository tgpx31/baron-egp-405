#include "Player.h"

#include "Sprite.h"

float timeElapsed = 0.0f;
float jumpTime = 1.0f;

Player::Player(Sprite * pSprite, const Vector2D & position, float orientation, float scale)
{
	mpSprite = pSprite;
	mPosition = position;
	mOrientation = orientation;
	mLocalScale = scale;

	mVel.setX(0);
	mVel.setY(0);

	mTarget = mPosition;
	mJumping = false;
}

Player::~Player()
{

}

void Player::update(float dt)
{
	mTarget = mPosition + mVel;
	mPosition = LerpVector2D(mPosition, mTarget, dt);
}

void Player::draw(GraphicsBuffer * pBuffer)
{
	mpSprite->draw(*pBuffer, mPosition.getX(), mPosition.getY(), mOrientation);
}
