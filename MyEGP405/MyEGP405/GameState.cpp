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
	strcpy(mGameStateData.board, "  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n");

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
		if (mGameStateData.selectedSpace >= 3)
		{
			mGameStateData.selectedSpace -= 3;
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

	case 'A':
		if (mGameStateData.selectedSpace >= 1 &&
			mGameStateData.selectedSpace != 3 &&
			mGameStateData.selectedSpace != 6)
		{
			mGameStateData.selectedSpace -= 1;
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

	case 'S':
		if (mGameStateData.selectedSpace <= 5)
		{
			mGameStateData.selectedSpace += 3;
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

	case 'D':
		if (mGameStateData.selectedSpace <= 7 &&
			mGameStateData.selectedSpace != 2 &&
			mGameStateData.selectedSpace != 5)
		{
			mGameStateData.selectedSpace += 1;
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

		// Enter: Check if valid space
	case '\0':
		if (validateMove())
		{
			// if valid move, update the board

		}
		else
		{
			printf("\n\tERROR: That space is taken!\n");
		}
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

int GameState::validateMove()
{
	// if the character at the selected space offset is not an underscore, return false
	if (*(mGameStateData.board + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) != '_')
		return 0;
	else
		return 1;

	return 0;
}