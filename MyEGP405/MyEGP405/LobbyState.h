#ifndef LOBBY_STATE_H
#define LOBBY_STATE_H

#include "State.h"

class LobbyState : public State
{
public:
	virtual void init(State* prev, State* nextL, State* nextR) override;
private:
	virtual void ArriveFromPreviousState(StateData *data) override;
	virtual void processBuffer() override;
	virtual void updateNetworking() {};
};

#endif