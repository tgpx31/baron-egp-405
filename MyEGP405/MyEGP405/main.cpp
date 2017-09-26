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

int main()
{
	
	//ClientState client[1];
	//ServerState server[1];
	//
	State *CurrentState;	// Starts in the Lobby State by default
	LobbyState lobby[1];

	lobby->init(nullptr, nullptr, nullptr, &CurrentState);

	CurrentState = lobby;

	// Lobby State
	// Start server
	// Or client?
	// ... or exit
	while (CurrentState->isRunning())
	{
		CurrentState->update();
	}
	system("pause");
	return 0;
}