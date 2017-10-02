#pragma once

#include "GameMessage.h"
#include "UI.h"

class ChangeDebugModeMessage : public GameMessage
{
public:
	ChangeDebugModeMessage(DebugMode mode);
	~ChangeDebugModeMessage();

	void process();

private:
	DebugMode mMode;
};