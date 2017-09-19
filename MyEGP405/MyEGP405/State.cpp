#include "State.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>

State::State()
{
	mData.running = 1;
	mData.port = 0;
	mData.bufferIndex = 0;
	mData.buffer[0] = '\0';
}

// Update input and data
void State::update()
{
	// update input
	// process buffer
	// update networking
	// update state data
	// display
	updateInput();
	processBuffer();
	updateNetworking();
	updateData();
}

// Render information to the screen
void State::render()
{
	system("CLS");
	std::cout << mData.buffer;
}

void State::init(State * prev, State * nextL, State * nextR, State ** currentState)
{
	mPrev = prev;
	mNextL = nextL;
	mNextR = nextR;
	mCurrentState = currentState;

	mData.running = 1;
}

State * State::operator=(State * other)
{
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
		}
	}

	// Alphabet
	for (i = 0x41; i < 0x5B; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < 256)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
		}
	}

	if (mData.keyboard[VK_BACK])
	{
		mData.buffer[--mData.bufferIndex] = '\0';
	}

	if (mData.keyboard[VK_RETURN])
	{
		processBuffer();
	}
}

// Process data currently in the input buffer, in the base state it just clears the buffer
void State::processBuffer()
{
	mData.buffer[0] = '\0';
	mData.bufferIndex = 0;
}

void State::GoToNextState(State* nextState)
{
	*mCurrentState = nextState;
	nextState->ArriveFromPreviousState(&mData);
}

void State::ArriveFromPreviousState(StateData * data)
{
	mData = *data;
}
