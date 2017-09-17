#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "ClientState.h"

class ServerState : public ClientState
{
public:
	void render();

private:
	void updateData();
	void updateNetworking();
	void processBuffer();
};

#endif