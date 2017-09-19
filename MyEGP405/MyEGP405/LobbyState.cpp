#include "LobbyState.h"
#include <stdio.h>

void LobbyState::init(State * prev, State * nextL, State * nextR)
{
	State::init(prev, nextL, nextR);

	mData.running = 0;
	mData.port = 0;
	mData.bufferIndex = 0;
	mData.buffer[0] = '\0';
	strcpy(mData.promptBuffer, "Welcome to the Lobby.\n(C)lient, (S)erver, or (E)xit?\n");

	mData.doesUpdateInput = 1;
	mData.doesUpdateNetworking = 0;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 0;
}

void LobbyState::updateNetworking()
{
}

// Process data currently in the input buffer
void LobbyState::processBuffer()
{
}

void LobbyState::ArriveFromPreviousState(StateData * data)
{
	render();
}
