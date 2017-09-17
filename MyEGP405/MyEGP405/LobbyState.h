#ifndef LOBBY_STATE_H
#define LOBBY_STATE_H

#include "State.h"

class LobbyState : public State
{
public:
	void render();

private:
	void updateData();
	void processBuffer();
};

#endif