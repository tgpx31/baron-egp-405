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
#include "NetworkedGameState.h"

int main()
{
	State *CurrentState;	// Starts in the Lobby State by default
	LobbyState lobby[1];
	GameState localGame[1];
	NetworkedGameState networkedGame[1];	// Can get away with using only one instance of this

	lobby->init(localGame, networkedGame, networkedGame, &CurrentState);
	localGame->init(lobby, &CurrentState);
	networkedGame->init(lobby, &CurrentState);
	
	CurrentState = lobby;

	while (CurrentState->isRunning())
	{
		CurrentState->update();
	}
	system("pause");
	return 0;
}