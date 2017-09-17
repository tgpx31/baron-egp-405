#include "ClientState.h"

// Update input, networking information, and data
void ClientState::update()
{
	updateInput();
	updateNetworking();
	updateData();
}

// Render information to the screen
void ClientState::render()
{
}

// Update data based on input and anything else
void ClientState::updateData()
{
}

// Receive and process incoming information from the network
void ClientState::updateNetworking()
{
}

// Process data currently in the input buffer
void ClientState::processBuffer()
{
}
