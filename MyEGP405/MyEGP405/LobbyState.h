#ifndef LOBBY_STATE_H
#define LOBBY_STATE_H

#include "State.h"

class LobbyState : public State
{
public:

	virtual void init(State* prev, State* nextL, State* nextR) override;

private:
	void updateNetworking();
	void processBuffer();

	virtual void ArriveFromPreviousState(StateData *data) override;
};

#endif