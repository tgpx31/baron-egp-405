#include "ServerState.h"

// Receive and process incoming information from the network
void ServerState::updateNetworking()
{
}

// Process data currently in the input buffer
void ServerState::processBuffer()
{
}

void ServerState::ArriveFromPreviousState(StateData * data)
{
	strcpy(mData.promptBuffer, "Please the port for your server: \n");
	render();
}
