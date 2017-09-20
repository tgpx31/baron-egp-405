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
	char str[512];

	const float framerate = 1.0f / 30.0f;

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

	{
		//	{
		//		// Server
		//		printf("Server selected\n");
		//		//ApplicationState->mData.state = SERVER_STATE;

		//		// Ask for port and max clients

		//		// Prompt for the port
		//		printf("\nInput Port Number: ");
		//		fgets(str, 512, stdin);

		//		// Grab the port number from the input
		//		sscanf(str, "%i", &ApplicationState->mData.port);
		//		printf("\nPort Number: %i \n", ApplicationState->mData.port);

		//		// If server, initialize maxClients
		//		printf("\nInput Max Clients: ");
		//		fgets(str, 512, stdin);

		//		// Grab the max clients from input
		//		sscanf(str, "%i", &maxClients);
		//		printf("\nMax Clients: %i \n", maxClients);

		//		RakNet::SocketDescriptor sd(ApplicationState->mData.port, 0);
		//		peer->Startup(maxClients, &sd, 1);

		//		peer->SetMaximumIncomingConnections(maxClients);
		//	}
		//	else // Invalid inputs
		//	{
		//		printf("Following input is invalid: %s(C)lient, (S)erver, (E)xit?\n", str);
		//	}

		//	// If it changes out of lobby state, it'll go into the network loop
		//	// Otherwise it will return to the Lobby State loop

		//	// Networking Loop
		//	while (ApplicationState->mData.running && ApplicationState->mData.state != LOBBY_STATE)
		//	{
		//		timer.start();

		//		ApplicationState->update();
		//		ApplicationState->render();

		//		timer.sleepUntilElapsed(framerate * 1000);
		//		timer.stop();
		//	}

		//}
	}
	system("pause");
	return 0;
}