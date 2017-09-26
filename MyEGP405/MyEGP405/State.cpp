// Brian Baron		0974390
// Justin Mulkin	0952465
//
// EGP 405-02	Lab 2	2017/09/25	(YYYY/MM/DD)
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


State::State()
{
	mData.doesUpdateInput = 0;
	mData.doesUpdateNetworking = 0;
	mData.doesUpdateState = 0;
	mData.doesDisplay = 0;

	mData.bufferIndex = 0;
	mData.buffer[0] = '\0';
	strcpy(mData.promptBuffer, "");

	mData.running = 1;
}
void State::init(State** currentState)
{
	mCurrentState = currentState;
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
	if (mData.keyboard[0x6E] || mData.keyboard[0xBE] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(0x6E, MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	/*if (mData.keyboard[VK_SPACE] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = ' ';
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}*/

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
	printf(mData.promptBuffer);
	printf(mData.buffer);
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