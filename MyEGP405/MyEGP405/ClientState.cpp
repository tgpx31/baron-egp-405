#include "ClientState.h"

// Receive and process incoming information from the network
void ClientState::updateNetworking()
{
}

void ClientState::updateData()
{
	int i;

	// Number keys
	for (i = 0x30; i < 0x3A; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < 256)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			render();
		}
	}

	// Alphabet
	for (i = 0x41; i < 0x5B; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < 256)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			render();
		}
	}

	if (mData.keyboard[VK_BACK])
	{
		if (mData.bufferIndex == 0)
			return;
		mData.buffer[--mData.bufferIndex] = '\0';
		render();
	}

	if (mData.keyboard[VK_RETURN])
	{
		processBuffer();
		memset(mData.buffer, 0, sizeof(char) * 256);
		mData.buffer[0] = '\0';
		mData.bufferIndex = 0;
	}
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

void ClientState::render()
{
	system("CLS");
	printf("%s%s", mData.promptBuffer, mData.buffer);
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
}
