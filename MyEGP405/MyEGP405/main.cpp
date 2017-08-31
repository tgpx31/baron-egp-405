#include <stdio.h>
#include "RakNet/RakPeerInterface.h"

// Using namespace to avoid doing Raknet:: a lot
using namespace RakNet;

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	unsigned int maxClients;
	unsigned short serverPort;

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);

	// TODO - Add code body here
	// initialize the port number regardless of client or host
	printf("\nInput Port Number: ");

	// Grab the port number from the input
	scanf("%hi", &serverPort);
	printf("\nPort Number: %hi \n", serverPort);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else 
	{
		// If server, initialize maxClients
		printf("\nInput Max Clients: ");
		
		// Grab the max clients from input
		scanf("%i", &maxClients);
		printf("\nMax Clients: %i \n", maxClients);

		SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}