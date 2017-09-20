#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "State.h"
#include "RakNet\BitStream.h"
#include "Messages.h"

class ClientState : public State
{
public:
	void init(State* prev, State* nextL, State* nextR, State** currentState) override;

protected:
	void updateNetworking();
	void processBuffer() override;

	void ArriveFromPreviousState(StateData *data) override;

	RakNet::RakPeerInterface *peer;
	RakNet::Packet *packet;

	char username[256];
	unsigned int id;

	//Flags
	int ipSet;
	int requestConnection;
};

#endif