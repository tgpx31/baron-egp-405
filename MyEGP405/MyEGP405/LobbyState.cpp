#include "LobbyState.h"
#include <stdio.h>

void LobbyState::init(State * prev, State * nextL, State * nextR)
{
	State::init(prev, nextL, nextR);

	mData.running = 1;
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
	switch (mData.buffer[0])
	{
	case 'C':
		printf("\nClient selected\n");
		// Initialize the client
		GoToNextState(mNextL);
		break;

	case 'E':
		printf("\nGoodbye\n");
		mData.running = 0;
		break;

	case 'S':
		printf("\nServer selected\n");
		// initialize the server
		GoToNextState(mNextR);
		break;


	default:
		printf("\n\tERROR: Invalid Input\n");
		break;

	}
	// Clear the buffer
	memset(mData.buffer, 0, sizeof(char) * 256);
	mData.buffer[0] = '\0';
	mData.bufferIndex = 0;
}

void LobbyState::ArriveFromPreviousState(StateData * data)
{
	render();
}
