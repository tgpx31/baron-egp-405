#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "ClientState.h"

class ServerState : public ClientState
{
public:

private:
	void updateNetworking();
	void processBuffer();
	void ArriveFromPreviousState(StateData *data) override;
};

#endif