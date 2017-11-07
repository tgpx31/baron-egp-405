#include "Event.h"
#include "NetworkedGameState.h"

void MovePlayerEvent::Execute()
{
	player->setVelocity(mVel);
}

void ExitGameEvent::Execute()
{
	mGame->endGame();
}

void FlipSpriteEvent::Execute()
{
	player->setSprite(sprite);
}

void ChangeScaleEvent::Execute()
{
	player->setScale(mScale);
}

void ChangeOrientationEvent::Execute()
{
	player->setOrientation(player->getOrientation() + mOrientation);
}
