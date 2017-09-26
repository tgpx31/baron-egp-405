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
#include "LobbyState.h"

void LobbyState::init(State * nextL, State * nextM, State * nextR, State** currentState)
{
	State::init(currentState);

	next1 = nextL;
	next2 = nextM;
	next3 = nextR;

	strcpy(mData.promptBuffer, "Welcome to Tic-Tac-Toe.\n\t(L)ocal game\n\t(H)ost an online game\n\t(C)onnect to an online game\n\t(E)xit?\n");

	mData.doesUpdateInput = 1;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 1;
}

// Process data currently in the input buffer
void LobbyState::processBuffer()
{
	switch (mData.buffer[0])
	{
	case 'C':
		printf("\nClient selected\n");
		// Initialize the client
		//GoToNextState(next3);
		break;

	case 'E':
		printf("\nGoodbye\n");
		mData.running = 0;
		break;

	case 'H':
		printf("\nHost selected\n");
		// initialize the server
		//GoToNextState(next2);
		break;

	case 'L':
		printf("\nLocal Game selected");
		// load a local game
		//GoToNextState(next1);
		break;

	default:
		printf("\n\tERROR: Invalid Input\n");
		break;

	}
	// Clear the buffer
	clearBuffer();
}