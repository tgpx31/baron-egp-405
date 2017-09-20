#include "ServerState.h"

void ServerState::init(State * prev, State * nextL, State * nextR, State ** currentState)
{
	State::init(prev, nextL, nextR, currentState);
	mData.doesUpdateInput = 1;
	mData.doesUpdateNetworking = 1;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 0;

	maxClients = 0;
	isServer = 0;

	peer = RakNet::RakPeerInterface::GetInstance();
	strcpy(mData.promptBuffer, "Please enter port number: \n");
}

// Receive and process incoming information from the network
void ServerState::updateNetworking()
{
	if (mData.port != 0 && maxClients > 0 && !isServer)
	{
		RakNet::SocketDescriptor sd(mData.port, 0);
		peer->Startup(maxClients, &sd, 1);
		peer->SetMaximumIncomingConnections(maxClients);
	}
}

// Process data currently in the input buffer
void ServerState::processBuffer()
{
	if (mData.port == 0)
	{
		sscanf(mData.buffer, "%i", &mData.port);
		// printf("\nPort Number: %i \n", mData.port);
		// Set the prompt to ask for max clients
		if (mData.port > 0)
			strcpy(mData.promptBuffer, "Please enter the maximum clients: \n");

		// Clear the buffer
		clearBuffer();
		render();
	}
	else if (mData.port != 0 && maxClients == 0)
	{
		sscanf(mData.buffer, "%i", &maxClients);

		if (maxClients > 0)
		{
			printf("\nPort Number: %i \n", mData.port);
			printf("\nMax Clients: %i \n", maxClients);
		}
		else
		{
			// Clear the buffer
			clearBuffer();
			render();
		}

		// Clear the buffer
		clearBuffer();
	}

	// Clear the buffer
	clearBuffer();
}

void ServerState::ArriveFromPreviousState(StateData * data)
{
	strcpy(mData.promptBuffer, "Please the port for your server: \n");
	mData.doesDisplay = 1;
}
