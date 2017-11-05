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

// This file wasmodified by Brian Baron, Colin Brady, and Justin Mulkin with permission from author.

#include "LobbyState.h"

void LobbyState::init(State * nextL, State * nextR, State** currentState)
{
	State::init(currentState);

	mLobbyData.next1 = nextL;
	mLobbyData.next2 = nextR;
	
	mLobbyData.promptHost = 0;

	strcpy(mData.promptBuffer, "The boids are back in town. \n\t(L)ocal boids\n\t(N)etworked Boids\n\t(E)xit?\n");

	mData.doesUpdateInput = 1;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 1;
}

// Process data currently in the input buffer
void LobbyState::processBuffer()
{
	if (!mLobbyData.promptHost)
	{
		switch (mData.buffer[0])
		{
		case 'L':
			printf("\nLocal boids selected");
			// load a local game
			GoToNextState(mLobbyData.next1);
			break;
		case 'N':
			strcpy(mData.promptBuffer, "(C)onnect or (H)ost?\n");
			mLobbyData.promptHost = 1;
			mData.dataMethod = 2;
			mData.doesDisplay = 1;
			break;

		case 'E':
			printf("\nGoodbye\n");
			mData.running = 0;
			break;

		default:
			printf("\n\tERROR: Invalid Input\n");
			break;
		}
	}
	else
	{
		switch (mData.buffer[0])
		{
		case 'C':
			printf("\Connect selected\n");
			// Initialize the client
			mData.mIsHost = 0;
			GoToNextState(mLobbyData.next2);
			break;

		case 'H':
			printf("\nHost selected\n");
			// initialize the server
			mData.mIsHost = 1;
			GoToNextState(mLobbyData.next2);
			break;
		}
	}
	// Clear the buffer
	clearBuffer();
}