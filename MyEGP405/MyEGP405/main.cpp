// Brian Baron		0974390
// Colin Brady		???????
// Justin Mulkin	???????
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



int main()
{
	State ApplicationState[1];	// Starts in the Lobby State by default
	Timer timer;
	char str[512];

	const float framerate = 1.0f / 30.0f;

	ApplicationState->mData.running = 1;
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	unsigned int maxClients;

	// Lobby State
	// Start server
	// Or client?
	// ... or exit
	printf("Welcome to the Lobby!\n(C)lient, (S)erver, (E)xit?\n");

	while (ApplicationState->mData.running && ApplicationState->mData.state == LOBBY_STATE)
	{
		fgets(str, 512, stdin);

		if (str[0] == 'E' || str[0] == 'e')
		{
			ApplicationState->mData.running = 0;
		}
		else if (str[0] == 'C' || str[0] == 'c')
		{
			// Client
			RakNet::SocketDescriptor sd;
			peer->Startup(1, &sd, 1);

			printf("Client selected\n");
			ApplicationState->mData.state = CLIENT_STATE;

			// Ask for IP and Port
			printf("Enter server IP or hit enter for 127.0.0.1\n");
			fgets(ApplicationState->mData.connectionAddress, 512, stdin);

			if (str[0] == '\n')
				strcpy(ApplicationState->mData.connectionAddress, "127.0.0.1");

			// Prompt for the port
			printf("\nInput Port Number: ");
			fgets(str, 512, stdin);

			// Grab the port number from the input
			sscanf(str, "%i", &ApplicationState->mData.port);
			printf("\nPort Number: %i \n", ApplicationState->mData.port);

			// attempt connection
			printf("Starting the client, connecting to %s.\n", ApplicationState->mData.connectionAddress);
			peer->Connect(ApplicationState->mData.connectionAddress, ApplicationState->mData.port, 0, 0);
		}
		else if (str[0] == 'S' || str[0] == 's')
		{
			// Server
			printf("Server selected\n");
			ApplicationState->mData.state = SERVER_STATE;

			// Ask for port and max clients

			// Prompt for the port
			printf("\nInput Port Number: ");
			fgets(str, 512, stdin);

			// Grab the port number from the input
			sscanf(str, "%i", &ApplicationState->mData.port);
			printf("\nPort Number: %i \n", ApplicationState->mData.port);

			// If server, initialize maxClients
			printf("\nInput Max Clients: ");
			fgets(str, 512, stdin);

			// Grab the max clients from input
			sscanf(str, "%i", &maxClients);
			printf("\nMax Clients: %i \n", maxClients);

			RakNet::SocketDescriptor sd(ApplicationState->mData.port, 0);
			peer->Startup(maxClients, &sd, 1);

			peer->SetMaximumIncomingConnections(maxClients);
		}
		else // Invalid inputs
		{
			printf("Following input is invalid: %s(C)lient, (S)erver, (E)xit?\n", str);
		}

		// If it changes out of lobby state, it'll go into the network loop
		// Otherwise it will return to the Lobby State loop

		// Networking Loop
		while (ApplicationState->mData.running && ApplicationState->mData.state != LOBBY_STATE)
		{
			timer.start();

			ApplicationState->update();
			ApplicationState->render();

			timer.sleepUntilElapsed(framerate * 1000);
			timer.stop();
		}

	}

	return 0;
}