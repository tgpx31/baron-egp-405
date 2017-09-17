#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "State.h"

class ClientState : public State
{
public:
	void update();
	void render();

private:
	void updateData();
	void updateNetworking();
	void processBuffer();
};

#endif