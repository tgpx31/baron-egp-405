#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "ClientState.h"

class ServerState : public ClientState
{
public:
	void init(State* prev, State* nextL, State* nextR, State** currentState) override;
private:
	void updateNetworking();
	void processBuffer() override;
	void ArriveFromPreviousState(StateData *data) override;
};

#endif