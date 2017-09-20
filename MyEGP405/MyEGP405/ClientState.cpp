#include "ClientState.h"

// Receive and process incoming information from the network
void ClientState::updateNetworking()
{
	//Attempt to connect if IP and the port number are set
	if (ipSet == 1 && mData.port != 0 && !requestConnection)
	{
		peer->Connect(mData.connectionAddress, mData.port, 0, 0);
		printf("Requesting connection...\n");
		requestConnection = 1;
	}
	else if (requestConnection)
	{
		// **** TODO
		// Message Loop
	}
}

// Process data currently in the input buffer
void ClientState::processBuffer()
{
	//If the connection address is still the default
	if (strcmp(mData.connectionAddress, "default\n") && ipSet == 0)
	{
		if (mData.buffer[0] == '\0')
			strcpy(mData.connectionAddress, "127.0.0.1");
		else //Else copy the inputted IP to the connection Address
			strcpy(mData.connectionAddress, mData.buffer);

		//Set the prompt to ask for the port number
		strcpy(mData.promptBuffer, "Please enter server port number: \n");

		// Clear the buffer
		clearBuffer();
		render();

		ipSet = 1;
	}
	//Else if the port number is still the default value
	else if(mData.port == 0 && ipSet == 1)
	{
		sscanf(mData.buffer, "%i", &mData.port);
		printf("\nIP Address: %s \n", mData.connectionAddress);
		printf("\nPort Number: %i \n", mData.port);
	}

	// Clear the buffer
	clearBuffer();
}

void ClientState::ArriveFromPreviousState(StateData *data)
{
	strcpy(mData.promptBuffer, "Please enter server IP: \n");
	mData.doesDisplay = 1;
}

//Initializing the client state
void ClientState::init(State* prev, State* nextL, State* nextR, State** currentState)
{
	State::init(prev, nextL, nextR, currentState);

	strcpy(username, "Guest_User");
	strcpy(mData.promptBuffer, "Please enter server IP: \n");

	mData.doesUpdateInput = 1;
	mData.doesUpdateNetworking = 1;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 0;

	ipSet = 0;
	requestConnection = 0;

	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);
}
