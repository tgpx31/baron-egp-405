#pragma once

#include "GameMessage.h"

class EndGameMessage :public GameMessage
{
public:
	EndGameMessage();
	~EndGameMessage();

	void process();
};