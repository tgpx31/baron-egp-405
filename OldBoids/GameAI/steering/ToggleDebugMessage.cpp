#include "ToggleDebugMessage.h"
#include "Game.h"

ToggleDebugMessage::ToggleDebugMessage() : GameMessage(TOGGLE_DEBUG_MESSAGE)
{
}

ToggleDebugMessage::~ToggleDebugMessage()
{
}

void ToggleDebugMessage::process()
{
	gpGame->toggleDebug();
}
