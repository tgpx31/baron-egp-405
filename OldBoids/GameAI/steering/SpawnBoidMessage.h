#pragma once

#include "GameMessage.h"
#include "Behaviors.h"

class SpawnBoidMessage : public GameMessage
{
public:
	SpawnBoidMessage();
	~SpawnBoidMessage();

	void process();
};