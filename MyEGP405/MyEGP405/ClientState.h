#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "State.h"

class ClientState : public State
{
public:
	void update();
	void render();

	void init(State* prev, State* nextL, State* nextR) override;

protected:
	void updateData();
	void updateNetworking();
	void processBuffer() override;

	void ArriveFromPreviousState(StateData *data) override;

	RakNet::RakPeerInterface *peer;
};

#endif