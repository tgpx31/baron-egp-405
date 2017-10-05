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
#include "GameState.h"


void GameState::init(State * prev = nullptr, State ** currentState = nullptr)
{
	if (currentState != nullptr)
		State::init(currentState);

	mData.doesUpdateInput = 1;
	mData.doesUpdateState = 1;
	mData.doesUpdateNetworking = 0;
	mData.doesDisplay = 0;

	if (prev != nullptr)
		mPrev = prev;

	strcpy(mData.promptBuffer, "BOIDS\n");

	initializedBoids = false;
	exit = false;
}

void GameState::updateData()
{
	//if (!mGameStateData.endGame)
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

		// ESC
		else if (mData.keyboard[0x1B] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(0x1B, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;

			processBuffer();
			clearBuffer();
		}

	}
	//else if (mGameStateData.endGame)
	{
		// R or M
		if (mData.keyboard[0x52] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(0x52, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
		else if (mData.keyboard[0x4D] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(0x4D, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	if (mData.keyboard[VK_RETURN])
	{
		processBuffer();
		clearBuffer();
	}
}

void GameState::processBuffer()
{
	//if (!mGameStateData.endGame)
	//{
	//	switch (mData.buffer[0])
	//	{
	//		// WASD: move the 'selected space'
	//		// **** TODO:
	//		// make sure the next selected space 
	//	case 'W':
	//	{
	//		if (mGameStateData.selectedSpace >= 3)
	//		{
	//			// reset the tmpBoard to the current board
	//			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

	//			mGameStateData.selectedSpace -= 3;

	//			// Change the char to the 'select char'
	//			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
	//			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
	//		}
	//		break;
	//	}
	//	case 'A':
	//	{
	//		if (mGameStateData.selectedSpace >= 1 &&
	//			mGameStateData.selectedSpace != 3 &&
	//			mGameStateData.selectedSpace != 6)
	//		{
	//			// reset the tmpBoard to the current board
	//			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

	//			mGameStateData.selectedSpace -= 1;

	//			// Change the char to the 'select char'
	//			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
	//			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
	//		}
	//		break;
	//	}
	//	case 'S':
	//	{
	//		if (mGameStateData.selectedSpace <= 5)
	//		{
	//			// reset the tmpBoard to the current board
	//			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

	//			mGameStateData.selectedSpace += 3;

	//			// Change the char to the 'select char'
	//			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
	//			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
	//		}
	//		break;
	//	}
	//	case 'D':
	//	{
	//		if (mGameStateData.selectedSpace <= 7 &&
	//			mGameStateData.selectedSpace != 2 &&
	//			mGameStateData.selectedSpace != 5)
	//		{
	//			// reset the tmpBoard to the current board
	//			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

	//			mGameStateData.selectedSpace += 1;

	//			// Change the char to the 'select char'
	//			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
	//			//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
	//		}
	//		break;
	//	}

	//	// ESC
	//	case '\x1b':
	//	{
	//		printf("ESC pressed\n");
	//		// Active player forfeits
	//		mGameStateData.endGame = 1;
	//		mGameStateData.winner = 1 - mGameStateData.playerPriority;

	//		strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");

	//		mData.doesDisplay = 1;
	//		break;
	//	}

	//	// Enter: Check if valid space
	//	case '\0':
	//		if (validateMove())
	//		{
	//			// if valid move, update the board
	//			// set the selected space to your char
	//			mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
	//			*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = mGameStateData.currentPlayerChar;

	//			// set the current board
	//			strcpy(mGameStateData.board, mGameStateData.tmpBoard);

	//			// check if player won
	//			if (checkWin())
	//			{
	//				mGameStateData.endGame = 1;
	//				mGameStateData.winner = mGameStateData.playerPriority;

	//				strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");
	//			}
	//			// check if draw
	//			else if (checkDraw())
	//			{
	//				mGameStateData.endGame = 1;
	//				mGameStateData.winner = -1;

	//				strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");
	//			}
	//			// if not, switch player priority
	//			else
	//			{
	//				mGameStateData.playerPriority = 1 - mGameStateData.playerPriority;
	//				mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
	//			}

	//			// reset selected space
	//			mGameStateData.selectedSpace = 4;
	//			mGameStateData.tmpBoard[mGameStateData.boardSpaceOffsets[4]] = '@';

	//			// Display
	//			mData.doesDisplay = 1;
	//		}
	//		else
	//		{
	//			printf("\n\tERROR: That space is taken!\n");
	//		}
	//		break;

	//	default:
	//		printf("\n\tERROR: Invalid Input\n");
	//		break;

	//	}
	//}
	//else if (mGameStateData.endGame)
	//{
	//	switch (mData.buffer[0])
	//	{
	//	case 'R':
	//	{
	//		strcpy(mData.promptBuffer, "* Tic-Tac-Toe * Use WASD to move your selection, press ENTER to confirm *\n*************************************************************************\n");
	//		strcpy(mGameStateData.board, "  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n");
	//		strcpy(mGameStateData.tmpBoard, mGameStateData.board);
	//		mGameStateData.tmpBoard[mGameStateData.boardSpaceOffsets[4]] = '@';

	//		// flip the starting player flag
	//		mGameStateData.initialPlayerPriority = 1 - mGameStateData.initialPlayerPriority;
	//		mGameStateData.playerPriority = mGameStateData.initialPlayerPriority;
	//		mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';

	//		mGameStateData.winner = -1;
	//		mGameStateData.endGame = 0;

	//		mData.doesDisplay = 1;
	//		break;
	//	}

	//	case 'M':
	//	{
	//		GoToNextState(mGameStateData.mPrev);
	//		break;
	//	}

	//	default:
	//		printf("\n\tERROR: Invalid Input\n");
	//		break;
	//	}
	//}
	// Clear the buffer
	clearBuffer();
}

void GameState::render()
{
	gpPerformanceTracker->clearTracker("loop");
	gpPerformanceTracker->startTracking("loop");

	gpGame->beginLoop();
	gpPerformanceTracker->clearTracker("draw");
	gpPerformanceTracker->startTracking("draw");

	gpGame->processLoop();
	gpPerformanceTracker->stopTracking("draw");

	exit = gpGame->endLoop();

	gpPerformanceTracker->stopTracking("loop");
	std::cout << "loop took:" << gpPerformanceTracker->getElapsedTime("loop") << "ms";
	std::cout << "draw took:" << gpPerformanceTracker->getElapsedTime("draw") << "ms\n";

	if (exit)
		mData.running = 0;
}

void GameState::ArriveFromPreviousState(StateData *data)
{
	init();
	mData.doesDisplay = 1;
	StartBoids();
}

int GameState::StartBoids()
{
	//gpGame = new Game();
	gpPerformanceTracker->startTracking("init");

	bool goodGame = gpGame->init();
	if (!goodGame)
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}
	else
	{
		gpPerformanceTracker->stopTracking("init");
		std::cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";

		initializedBoids = true;
		return 1;
	}

	return 0;
}