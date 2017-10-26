#include "Game.h"
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"

PlayerMoveToMessage::PlayerMoveToMessage(const Vector2D& pos)
	:GameMessage(PLAYER_MOVETO_MESSAGE)
	, mPos(pos)
{
}

PlayerMoveToMessage::~PlayerMoveToMessage()
{
}

void PlayerMoveToMessage::process()
{
	// interpolate to target position
	// add get player to Game
}

