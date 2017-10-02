#include "EndGameMessage.h"
#include "Game.h"

EndGameMessage::EndGameMessage() : GameMessage(END_GAME_MESSAGE)
{
}

EndGameMessage::~EndGameMessage()
{
}

void EndGameMessage::process()
{
	gpGame->endGame();
}
