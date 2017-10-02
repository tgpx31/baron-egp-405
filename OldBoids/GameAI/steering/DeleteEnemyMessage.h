#pragma once

#include "GameMessage.h"

class DeleteEnemyMessage : public GameMessage
{
public:
	DeleteEnemyMessage();
	~DeleteEnemyMessage();

	void process();
};