// Brian Baron	0974390
// EGP 405-02	Lab 1: Hello, RakNet	2017/09/01 (YYYY/MM/DD)
//
//
//--------------------------------------------------------------------------------------------------------------------------------
//		 We certify that this work is entirely our own.The assessor of this project may reproduce this project
//		 and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism-checking service,
//		 which may retain a copy of the project on its database
//--------------------------------------------------------------------------------------------------------------------------------
//
//
// This file was modified by Brian Baron with permission from author
// (I put this because of the tutorial source code just in case)
// Since this was not a team effort, all code modified/written for this Lab was Brian Baron
//
// Repository Link: https://github.com/tgpx31/baron-egp-405
//
//
// Sources for tutorials listed in the Lab 1 assignment document:
// http://www.jenkinssoftware.com/raknet/manual/tutorialsample1.html
// http://www.jenkinssoftware.com/raknet/manual/tutorialsample2.html
// http://www.jenkinssoftware.com/raknet/manual/tutorialsample3.html
// http://www.raknet.net/raknet/manual/creatingpackets.html
//

#include <stdio.h>
#include <string.h>
#include "RakNet\RakPeerInterface.h"

// Part b
#include "RakNet\MessageIdentifiers.h"

// Part c
#include "RakNet\BitStream.h"
#include "RakNet\RakNetTypes.h"

// Using namespace to avoid doing Raknet:: a lot
using namespace RakNet;

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	// Custom Message Identifiers for identifying packets
	ID_DISDAIN_FOR_THE_PEOPLE,
	ID_KICK_PEASANT,
};

// struct to replace bitstream
#pragma pack(push, 1)
struct ScreamingMessageStruct
{
	unsigned char mTypeId;	// Used to ID the type of message

	// string containing message
	char mMessage[64] = "Goodbye peasant";
};
#pragma pack(pop)

// Container functions for ease of use to recieve and send messages
void recieveMessage(Packet* packet);
void sendMessage(const char* msg, MessageID id, RakPeerInterface *peer, Packet *packet);

//---------------------
int main(void)
{
	char str[512];
	RakPeerInterface *peer = RakPeerInterface::GetInstance();
	bool isServer;

	unsigned int maxClients;
	unsigned short serverPort;
	Packet *packet;

	// Prompt for the port
	printf("\nInput Port Number: ");
	fgets(str, 512, stdin);

	// Grab the port number from the input
	sscanf(str, "%hi", &serverPort);
	printf("\nPort Number: %hi \n", serverPort);

	// Server or client?
	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);

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
		fgets(str, 512, stdin);

		// Grab the max clients from input
		sscanf(str, "%i", &maxClients);
		printf("\nMax Clients: %i \n", maxClients);

		SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// Set client limit
		peer->SetMaximumIncomingConnections(maxClients);
	}
	else
	{
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		fgets(str, 512, stdin);

		// check if the string is empty
		if (str[0] == '\n') {
			strcpy(str, "127.0.0.1");
		}

		// attempt connection
		printf("Starting the client, connecting to %s.\n", str);
		peer->Connect(str, serverPort, 0, 0);

	}

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;

			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;

			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");
				sendMessage("Please sir, can I have some packets", (MessageID)ID_DISDAIN_FOR_THE_PEOPLE, peer, packet);
				break;
			}

			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A connection is incoming.\n");
				break;
			}

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer)
				{
					printf("A client has disconnected.\n");
					break;
				}
				else
				{
					printf("\tWe have been disconnected.\n");
					break;
				}

			case ID_CONNECTION_LOST:
				if (isServer)
				{
					printf("A client lost the connection.\n");
					break;
				}
				else
				{
					printf("Connection lost.\n");
					break;
				}

			case ID_DISDAIN_FOR_THE_PEOPLE:
			{
				recieveMessage(packet);
				sendMessage("Gross, peasants!", (MessageID)ID_GAME_MESSAGE_1, peer, packet);
				break;
			}

			case ID_GAME_MESSAGE_1:
			{
				recieveMessage(packet);
				sendMessage("Down with the bourgeoisie!", (MessageID)ID_KICK_PEASANT, peer, packet);
				break;
			}

			case ID_KICK_PEASANT:
				recieveMessage(packet);

				if (isServer)
				{
					// disconnect the peasants
					sendMessage("Die, peasants!", (MessageID)ID_DISCONNECTION_NOTIFICATION, peer, packet);
					printf("\tKicking the clients.\n");

					peer->CloseConnection(packet->systemAddress, true, 0, HIGH_PRIORITY);
				}
				break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}
//---------------------
void recieveMessage(Packet* packet)
{
	ScreamingMessageStruct *pmsIn;
	pmsIn = (ScreamingMessageStruct*)packet->data;
	printf("Them: %s\n", pmsIn->mMessage);
}

void sendMessage(const char* msg, MessageID id, RakPeerInterface *peer, Packet *packet)
{
	ScreamingMessageStruct msOut;
	strcpy(msOut.mMessage, msg);
	msOut.mTypeId = id;
	printf("Me: %s\n", msOut.mMessage);

	// cast that struct, better way for this lab
	peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}