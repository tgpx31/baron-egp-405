#include "State.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>

State::State()
{
	mData.running = 0;
	mData.port = 0;
	mData.bufferIndex = 0;
	mData.buffer[0] = '\0';
	strcpy(mData.promptBuffer, "");

	mData.doesUpdateInput = 0;
	mData.doesUpdateNetworking = 0;
	mData.doesUpdateState = 0;
	mData.doesDisplay = 0;

	strcpy(mData.connectionAddress, "default");
}

void State::init(State * prev, State * nextL, State * nextR, State **currentState)
{
	mPrev = prev;
	mNextL = nextL;
	mNextR = nextR;
	mCurrentState = currentState;

	mData.running = 1;
}
State * State::operator=(State * other)
{
	// Point to the other state
	mData.running = other->mData.running;
	mData.port = other->mData.port;
	mData.bufferIndex = other->mData.bufferIndex;
	strcpy(mData.buffer, other->mData.buffer);

	// Run Arrival
	other->ArriveFromPreviousState(&mData);
	return other;
}


// Update current input state
void State::updateInput()
{
	//GetKeyboardState(mData.keyboard);

	for (int i = 0; i < 256; ++i)
	{
		// This creates problems, like not being able to use shift/caplock propery, but we're going to go with it for now
		mData.keyboard[i] = GetAsyncKeyState(i);
	}
}
// Update data based on input and anything else
void State::updateData()
{
	int i;

	// Number keys
	for (i = 0x30; i < 0x3A; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < 256)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	// Alphabet
	for (i = 0x41; i < 0x5B; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < 256)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	if (mData.keyboard[VK_BACK])
	{
		if (mData.bufferIndex == 0)
			return;
		mData.buffer[--mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	if (mData.keyboard[VK_RETURN])
	{
		processBuffer();
		memset(mData.buffer, 0, sizeof(char) * 256);
		mData.buffer[0] = '\0';
		mData.bufferIndex = 0;
	}
}
// Process data currently in the input buffer, in the base state it just clears the buffer
void State::processBuffer()
{
	mData.buffer[0] = '\0';
	mData.bufferIndex = 0;
}

// Update input and data
void State::update()
{
	// update input
	// update networking
	// update state data
	// display
	if (mData.doesUpdateInput)
		updateInput();

	if (mData.doesUpdateNetworking)
		updateNetworking();
	
	if (mData.doesUpdateState)
		updateData();

	if (mData.doesDisplay)
		render();
}
// Render information to the screen
void State::render()
{
	system("CLS");
	std::cout << mData.promptBuffer << mData.buffer;
	mData.doesDisplay = 0;
}



void State::GoToNextState(State* nextState)
{
	*mCurrentState = nextState;
	nextState->ArriveFromPreviousState(&mData);
}
void State::ArriveFromPreviousState(StateData * data)
{
	printf("YOU MADE IT");
	mData = *data;
}