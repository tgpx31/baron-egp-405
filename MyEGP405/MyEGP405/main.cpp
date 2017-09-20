// Brian Baron		0974390
// Colin Brady		0979605
// Justin Mulkin	0952465
//
// EGP 405-02	Project 1	2017/09/17 (YYYY/MM/DD)
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

#include <stdio.h>
#include "State.h"
#include "Timer.h"
#include "LobbyState.h"
#include "ClientState.h"
#include "ServerState.h"


int main()
{
	
	LobbyState lobby[1];
	ClientState client[1];
	ServerState server[1];
	
	State *CurrentState;	// Starts in the Lobby State by default

	lobby->init(nullptr, client, server, &CurrentState);
	client->init(lobby, nullptr, nullptr, &CurrentState);
	server->init(lobby, nullptr, nullptr, &CurrentState);

	CurrentState = lobby;
	Timer timer;

	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	unsigned int maxClients;

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