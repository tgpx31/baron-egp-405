#include "GameState.h"

void GameState::init(State * prev, State ** currentState)
{
	State::init(currentState);

	mData.doesUpdateInput = 1;
	mData.doesUpdateState = 1;
	mData.doesUpdateNetworking = 0;
	mData.doesDisplay = 0;

	mGameStateData.mPrev = prev;

	strcpy(mData.promptBuffer, "* Tic-Tac-Toe * Use WASD to move your selection, press ENTER to confirm *\n*************************************************************************\n");
	initializeBoard();

	mGameStateData.initialPlayerPriority = 1;	// start with X
	mGameStateData.playerPriority = 1;	// start with X
	mGameStateData.selectedSpace = 4;
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
			break;

		if (c == '_')
		{
			// store the offset from the start of the board buffer
			mGameStateData.boardSpaceOffsets[i] = &c - &mGameStateData.board[0];

			++i;
		}
	}
	mGameStateData.tmpBoard[mGameStateData.boardSpaceOffsets[4]] = '@';
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
			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
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
			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
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
			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
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
			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
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
			if (checkWin())
			{
				mGameStateData.winner = mGameStateData.playerPriority;
				printf("Player %c is the winner!\n", mGameStateData.currentPlayerChar);
				// set the tmpBoard to an empty string and wait for user to select option
				// new round or
				// return to menu state


				// for now, reset the board
				strcpy(mGameStateData.board, "  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n");
				strcpy(mGameStateData.tmpBoard, mGameStateData.board);
				mGameStateData.tmpBoard[mGameStateData.boardSpaceOffsets[4]] = '@';

				// flip the starting player flag
				mGameStateData.initialPlayerPriority = 1 - mGameStateData.initialPlayerPriority;
				mGameStateData.playerPriority = mGameStateData.initialPlayerPriority;
			}
			// check if draw
			else if (checkDraw())
			{
				printf("It's a draw!\n");
				// set the tmpBoard to an empty string and wait for user to select option
				// new round or
				// return to menu state


				// for now, reset the board
				strcpy(mGameStateData.board, "  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n");
				strcpy(mGameStateData.tmpBoard, mGameStateData.board);
				mGameStateData.tmpBoard[mGameStateData.boardSpaceOffsets[4]] = '@';

				// flip the starting player flag
				mGameStateData.initialPlayerPriority = 1 - mGameStateData.initialPlayerPriority;
				mGameStateData.playerPriority = mGameStateData.initialPlayerPriority;
			}
			// if not, switch player priority
			else
			{
				mGameStateData.playerPriority = 1 - mGameStateData.playerPriority;
				mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
			}

			// reset selected space
			mGameStateData.selectedSpace = 4;
			mData.doesDisplay = 1;
			mGameStateData.tmpBoard[mGameStateData.boardSpaceOffsets[4]] = '@';
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

	printf("\nPlayer %c's turn\n", mGameStateData.currentPlayerChar);
	//printf(mData.buffer);
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

int GameState::checkWin()
{
	// If we got here, then our move was validated and we placed our char
	
	// check horizontal
	{
		// if selected space is in the left column
		if (mGameStateData.selectedSpace == 0 || mGameStateData.selectedSpace % 3 == 0)
		{
			if (*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace + 1]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace + 2]) == mGameStateData.currentPlayerChar)
			{
				return 1;
			}
		}
		// middle
		else if (mGameStateData.selectedSpace % 3 == 1)
		{
			if (*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace - 1]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace + 1]) == mGameStateData.currentPlayerChar)
			{
				return 1;
			}
		}
		// right
		else if (mGameStateData.selectedSpace % 3 == 2)
		{
			if (*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace - 2]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace - 1]) == mGameStateData.currentPlayerChar)
			{
				return 1;
			}
		}
	}
	// check vertical
	{
		// top row
		if (mGameStateData.selectedSpace >= 0 && mGameStateData.selectedSpace <= 2)
		{
			if (*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace + 3]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace + 6]) == mGameStateData.currentPlayerChar)
			{
				return 1;
			}
		}
		// middle
		else if (mGameStateData.selectedSpace >= 3 && mGameStateData.selectedSpace <= 5)
		{
			if (*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace - 3]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace + 3]) == mGameStateData.currentPlayerChar)
			{
				return 1;
			}
		}
		// bottom
		else if (mGameStateData.selectedSpace >= 6 && mGameStateData.selectedSpace <= 8)
		{
			if (*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace - 6]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace - 3]) == mGameStateData.currentPlayerChar)
			{
				return 1;
			}
		}
	}
	// check diagonals
	{
		if ((*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[0]) == mGameStateData.currentPlayerChar &&
			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[4]) == mGameStateData.currentPlayerChar &&
			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[8]) == mGameStateData.currentPlayerChar) 
			||
			(*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[2]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[4]) == mGameStateData.currentPlayerChar &&
				*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[6]) == mGameStateData.currentPlayerChar))
		{
			return 1;
		}
	}

	return 0;
}

int GameState::checkDraw()
{
	// Check if all spaces are filled
	for (unsigned int i = 0; i < 9; ++i)
	{
		// if there are still empty spaces, there is no draw
		if (*(mGameStateData.board + mGameStateData.boardSpaceOffsets[i]) == '_')
			return 0;	// no draw
	}

	return 1;	// draw
}