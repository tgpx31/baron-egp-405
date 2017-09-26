#include "GameState.h"

void GameState::init(State * prev, State ** currentState)
{
	State::init(currentState);
	mGameStateData.mPrev = prev;

	strcpy(mData.promptBuffer, "Tic-Tac-Toe*\n************\n");
	initializeBoard();

	mGameStateData.playerPriority = 1;	// start with X
	mGameStateData.selectedSpace = 0;
	mGameStateData.currentPlayerChar = 'X';
	mGameStateData.winner = -1;
}

void GameState::initializeBoard()
{
	strcpy(mGameStateData.board, "  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  ");

	// I do it like this in case in the creation of this project I reorganize the buffer.
	// Otherwise values would be hard set
	unsigned int i = 0;
	for (char& c : mGameStateData.board)
	{
		// if the char is an underscore, it represents an empty space
		// if there is room in the empty spaces array, store the offset of that space.
		if (i >= BOARD_SLOTS)
			return;

		if (c == '_')
		{
			// store the offset from the start of the board buffer
			mGameStateData.boardSpaceOffsets[i] = &c - &mGameStateData.board[0];

			++i;
		}
	}
}


void GameState::updateData()
{
	// WASD
	if (mData.keyboard['W'] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey('W', MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;

		processBuffer();
		clearBuffer();
	}
	else if (mData.keyboard['A'] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey('A', MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;

		processBuffer();
		clearBuffer();
	}
	else if (mData.keyboard['S'] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey('S', MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;

		processBuffer();
		clearBuffer();
	}
	else if (mData.keyboard['D'] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey('D', MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;

		processBuffer();
		clearBuffer();
	}

	if (mData.keyboard[VK_RETURN])
	{
		processBuffer();
		clearBuffer();
	}
}

void GameState::processBuffer()
{
	switch (mData.buffer[0])
	{
		// WASD: move the 'selected space'
		// **** TODO:
		// make sure the next selected space 
	case 'W':
		mGameStateData.selectedSpace -= 3;
		break;

	case 'A':
		mGameStateData.selectedSpace -= 1;
		break;

	case 'S':
		mGameStateData.selectedSpace += 3;
		break;

	case 'D':
		mGameStateData.selectedSpace += 1;
		break;

		// Enter: Check if valid space
	case '\0':
		break;

	default:
		printf("\n\tERROR: Invalid Input\n");
		break;

	}
	// Clear the buffer
	clearBuffer();
}






void GameState::render()
{
	system("CLS");
	printf(mData.promptBuffer);
	// Display the board
	printf(mGameStateData.board);

	printf(mData.buffer);
	mData.doesDisplay = 0;
}

void GameState::ArriveFromPreviousState(StateData *data)
{
	// Render the prompt to screen
	mData.doesDisplay = 1;
}
