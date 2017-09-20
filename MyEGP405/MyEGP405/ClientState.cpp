#include "ClientState.h"

// Receive and process incoming information from the network
void ClientState::updateNetworking()
{
	//Attempt to connect if IP and the port number are set
	if (ipSet == 1 && mData.port != 0 && !requestConnection)
	{
		peer->Connect(mData.connectionAddress, mData.port, 0, 0);
		requestConnection = 1;
	}
}

// Process data currently in the input buffer
void ClientState::processBuffer()
{
	//If the connection address is still the default
	if (strcmp(mData.connectionAddress, "default\n") && ipSet == 0)
	//if (mData.connectionAddress == "default")
	{
		//If this is an endline character, pick default IP
		if (mData.buffer[0] == '\n')
			strcpy(mData.connectionAddress, "127.0.0.1");
		else //Else copy the inputted IP to the connection Address
			strcpy(mData.connectionAddress, mData.buffer);

		//Set the prompt to ask for the port number
		strcpy(mData.promptBuffer, "Please enter server port number: \n");

		// Clear the buffer
		memset(mData.buffer, 0, sizeof(char) * 256);
		mData.buffer[0] = '\0';
		mData.bufferIndex = 0;

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
	memset(mData.buffer, 0, sizeof(char) * 256);
	mData.buffer[0] = '\0';
	mData.bufferIndex = 0;
}

void ClientState::ArriveFromPreviousState(StateData *data)
{
	//mData = *data;
	strcpy(mData.promptBuffer, "Please enter server IP: \n");
	render();
}

//Initializing the client state
void ClientState::init(State* prev, State* nextL, State* nextR, State** currentState)
{
	State::init(prev, nextL, nextR, currentState);

	strcpy(mData.promptBuffer, "Please enter server IP: \n");

	mData.doesUpdateInput = 1;
	mData.doesUpdateNetworking = 0;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 0;

	ipSet = 0;
	requestConnection = 0;
}
