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

// My 'hi, goodbye' loop
// TODO:	client says hi to server
//			server says they'll remove the client in x seconds
//			after x seconds, say bye between both
//			wait one second
//			force client disconnect

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	ID_GAME_MESSAGE_2,
	ID_SET_TIMED_DISCONNECT,
};

// struct to replace bitstream
#pragma pack(push, 1)
struct MessageStruct
{
	unsigned char mTimeStampID;		// Assign ID based on timestamp
	Time mTimeStamp;				// Assigned by GetTime()
	unsigned char mTypeId;			// Type here

	// string containing message
	char mMessage[512];
};
#pragma pack(pop)

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	unsigned int maxClients;
	unsigned short serverPort;

	Packet *packet;

	printf("\nInput Port Number: ");
	fgets(str, 512, stdin);

	// Grab the port number from the input
	sscanf(str, "%hi", &serverPort);
	printf("\nPort Number: %hi \n", serverPort);

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

	//  Start tutorial sample 2 work
	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
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

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				BitStream bsOut;
				bsOut.Write((MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello!");
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				break;
			case ID_NEW_INCOMING_CONNECTION:
			{
				printf("A connection is incoming.\n");
				/*BitStream bsOut;
				bsOut.Write((MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello to you too!");
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);*/
				
				break;
			}
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer)
				{
					printf("A client has disconnected.\n");
				}
				else
				{
					printf("We have been disconnected.\n");

					// say bye, never triggers this switch case though
					BitStream bsOut;
					bsOut.Write((MessageID)ID_GAME_MESSAGE_2);
					bsOut.Write("Goodbye!");
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer)
				{
					printf("A client lost the connection.\n");
				}
				else
				{
					printf("Connection lost.\n");
				}
				break;
			case ID_GAME_MESSAGE_1:
			{
				RakString rs;
				BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());

				// say bye
				BitStream bsOut;
				bsOut.Write((MessageID)ID_GAME_MESSAGE_2);
				bsOut.Write("Goodbye!");
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
			case ID_GAME_MESSAGE_2:
			{
				RakString rs;
				BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());

				// Say hi again to keep a loop
				BitStream bsOut;
				bsOut.Write((MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello!");
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
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