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
	//If the connection address is still the default
	if (mData.connectionAddress == "")
	{
		//If this is an endline character, pick default IP
		if (mData.buffer[0] == '\n')
			strcpy(mData.connectionAddress, "127.0.0.1");
		else //Else copy the inputted IP to the connection Address
			strcpy(mData.connectionAddress, mData.buffer);

		//Set the prompt to ask for the port number
		strcpy(mData.promptBuffer, "Please enter server port number: \n");
	}
	//Else if the port number is still the default value
	else if(mData.port == 0)
	{
		sscanf(mData.buffer, "%i", &mData.port);
		printf("\nPort Number: %i \n", mData.port);
	}

	// Clear the buffer
	memset(mData.buffer, 0, sizeof(char) * 256);
	mData.buffer[0] = '\0';
	mData.bufferIndex = 0;
}

void ClientState::ArriveFromPreviousState(StateData *data)
{

}

//Initializing the client state
void ClientState::init(State* prev, State* nextL, State* nextR)
{
	State::init(prev, nextL, nextR);

	strcpy(mData.promptBuffer, "Please enter server IP: \n");


}
