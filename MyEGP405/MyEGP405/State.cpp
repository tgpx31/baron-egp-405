// Brian Baron		0974390
// Colin Brady		0979605
// Justin Mulkin	0952465
//
// EGP 405-02	Project 1	2017/09/17 (YYYY/MM/DD)
//
//
//--------------------------------------------------------------------------------------------------------------------------------
//		 We certify that this work is entirely our own.The assessor of this project may reproduce this project
//		 and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism-checking service,
//		 which may retain a copy of the project on its database
//--------------------------------------------------------------------------------------------------------------------------------
//
// Repository Link: https://github.com/tgpx31/baron-egp-405
//
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


// Update current input state
void State::updateInput()
{
	//GetKeyboardState(mData.keyboard);
	// new tech, only get input if active window
	if (GetConsoleWindow() != GetForegroundWindow())
		return;

	for (int i = 0; i < STR_MAX; ++i)
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
		if (mData.keyboard[i] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	// Numpad
	for (i = 0x60; i < 0x6A; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	// Decimal key/period
	i = 0x6E;
	if (mData.keyboard[0x6E] || mData.keyboard[0xBE] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	if (mData.keyboard[VK_SPACE] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = ' ';
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	// Alphabet
	for (i = 0x41; i < 0x5B; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < STR_MAX)
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
		clearBuffer();
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


// traverse between states
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

void State::clearBuffer()
{
	// Clear the buffer
	memset(mData.buffer, 0, sizeof(char) * STR_MAX);
	mData.buffer[0] = '\0';
	mData.bufferIndex = 0;
}
