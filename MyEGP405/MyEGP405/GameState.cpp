#include "GameState.h"

void GameState::init(State * prev, State ** currentState)
{
	State::init(currentState);
	mGameStateData.mPrev = prev;
	mGameStateData.isLocal = 1;

	strcpy(mData.promptBuffer, "Tic-Tac-Toe*\n************\n");
	initializeBoard();
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
