#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "State.h"

class ClientState : public State
{
public:
	void init(State* prev, State* nextL, State* nextR, State** currentState) override;

protected:
	void updateNetworking();
	void updateData() override;
	void processBuffer() override;
	void render() override;

	void ArriveFromPreviousState(StateData *data) override;

	RakNet::RakPeerInterface *peer;

	//Flags
	int ipSet;
};

#endif