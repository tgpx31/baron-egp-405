#pragma once

#include "GameMessage.h"

class ToggleDebugMessage : public GameMessage
{
private:

public:
	ToggleDebugMessage();
	~ToggleDebugMessage();

	void process();
};