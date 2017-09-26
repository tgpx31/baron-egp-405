#include "GameState.h"

void GameState::init(State * prev, State ** currentState)
{
	State::init(currentState);

	mData.doesUpdateInput = 1;
	mData.doesUpdateState = 1;
	mData.doesUpdateNetworking = 0;
	mData.doesDisplay = 0;

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
	strcpy(mGameStateData.tmpBoard, mGameStateData.board);
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
	if (mData.keyboard[0x57] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(0x57, MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;

		processBuffer();
		clearBuffer();
	}
	else if (mData.keyboard[0x41] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(0x41, MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;

		processBuffer();
		clearBuffer();
	}
	else if (mData.keyboard[0x53] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(0x53, MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;

		processBuffer();
		clearBuffer();
	}
	else if (mData.keyboard[0x44] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(0x44, MAPVK_VK_TO_CHAR);
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
			// reset the tmpBoard to the current board
			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

			mGameStateData.selectedSpace -= 3;

			// Change the char to the 'select char'
			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

	case 'A':
		if (mGameStateData.selectedSpace >= 1 &&
			mGameStateData.selectedSpace != 3 &&
			mGameStateData.selectedSpace != 6)
		{
			// reset the tmpBoard to the current board
			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

			mGameStateData.selectedSpace -= 1;

			// Change the char to the 'select char'
			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

	case 'S':
		if (mGameStateData.selectedSpace <= 5)
		{
			// reset the tmpBoard to the current board
			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

			mGameStateData.selectedSpace += 3;

			// Change the char to the 'select char'
			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

	case 'D':
		if (mGameStateData.selectedSpace <= 7 &&
			mGameStateData.selectedSpace != 2 &&
			mGameStateData.selectedSpace != 5)
		{
			// reset the tmpBoard to the current board
			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

			mGameStateData.selectedSpace += 1;

			// Change the char to the 'select char'
			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
			printf("\nSelected space %i\n", mGameStateData.selectedSpace);
		}
		break;

		// Enter: Check if valid space
	case '\0':
		if (validateMove())
		{
			// if valid move, update the board
			// set the selected space to your char
			mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = mGameStateData.currentPlayerChar;
			// set the current board
			strcpy(mGameStateData.board, mGameStateData.tmpBoard);
			// check if player won


			// if not, switch player priority

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
	// if the boards are different, display the tmpBoard
	// otherwise display the regular board
	if (strcmp(mGameStateData.board, mGameStateData.tmpBoard) != 0)
	{
		printf(mGameStateData.tmpBoard);
	}
	else
	{
		printf(mGameStateData.board);
	}

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
		return 0;	// false
	else
		return 1;	// true

	return 0;	// false
}