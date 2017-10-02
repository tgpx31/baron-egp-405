#pragma once

#include "GameMessage.h"
#include "Behaviors.h"


class SpawnEnemyMessage : public GameMessage
{
private:
	Behavior mBehavior;

public:
	SpawnEnemyMessage(Behavior behavior);
	~SpawnEnemyMessage();

	void process();
};
