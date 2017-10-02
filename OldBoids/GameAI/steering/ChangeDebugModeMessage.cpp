#include "ChangeDebugModeMessage.h"
#include "Game.h"

ChangeDebugModeMessage::ChangeDebugModeMessage(DebugMode mode) : GameMessage(CHANGE_UI_MODE_MESSAGE)
{
	mMode = mode;
}

ChangeDebugModeMessage::~ChangeDebugModeMessage()
{
}

void ChangeDebugModeMessage::process()
{
	gpGame->getUI()->setMode(mMode);
}
