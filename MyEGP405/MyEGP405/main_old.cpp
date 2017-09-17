#include <stdio.h>
#include <string.h>
#include "RakNet\RakPeerInterface.h"
#include "RakNet\BitStream.h"
#include "RakNet\RakNetTypes.h" 

#include "Messages.h"

int notmain(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	unsigned int maxClients = 10;
	unsigned int serverPort = 1111;
	RakNet::Packet *packet;

	printf("(C) or (S)erver?\n");
	fgets(str, 512, stdin);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else 
	{
		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(maxClients);
	}
	else {
		/*
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		fgets(str, 512, stdin);
		if (str[0] == '\n') {
			strcpy(str, "127.0.0.1");
		}
		*/
		strcpy(str, "127.0.0.1");
		printf("Starting the client.\n");
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

			// Client successfully joins server
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");
				/*
				// client recieves
				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				
				// METHOD 1: pack using bitstream
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_HELLO_MESSAGE);
				bsOut.Write("Hello world from client");
				//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				// METHOD 2
				MessageStruct ms = { ID_HELLO_MESSAGE, "Hello from client" };
				peer->Send((char*)&ms, sizeof(ms), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				*/

				// prompt for username input
				// temp hardcode
				UsernameMessage username[1] = { ID_USERNAME, "HardCodedName" };
				peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				break;

			case ID_USERNAME:
			{
				// we are server
				// store UN in dict
				// broadcast client entry to all other clients
				UsernameMessage *username = (UsernameMessage*)packet->data;
				username->messageID = ID_NEW_CLIENT_JOIN;
				// send entry message to all except the client that joined
				peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);

				// send new client their id
				// should set ID based on clients currently connected
				ClientNumberMessage clientNumber[1] = { ID_CLIENT_NUMBER, 0 };
				peer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;

			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) {
					printf("A client has disconnected.\n");
				}
				else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer) {
					printf("A client lost the connection.\n");
				}
				else {
					printf("Connection lost.\n");
				}
				break;
			/*
			//case ID_HELLO_MESSAGE:
			//{
			//	// anyone can recieve this message
			//	RakNet::RakString rs;
			//	RakNet::BitStream bsIn(packet->data, packet->length, false);
			//	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			//	bsIn.Read(rs);
			//	printf("%s\n", rs.C_String());

			//	// METHOD 1: pack using bitstream
			//	//RakNet::BitStream bsOut;
			//	//bsOut.Write((RakNet::MessageID)ID_HELLO_MESSAGE);
			//	//if (isServer)
			//	//	bsOut.Write("Hello world from server");
			//	//else
			//	//	bsOut.Write("Hello world from client");
			//	//// emulating TCP
			//	//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

			//	MessageStruct* pmsIn;
			//	pmsIn = (MessageStruct*)packet->data;
			//	printf("%s\n", pmsIn->message);

			//	MessageStruct ms;
			//	if (isServer)
			//		ms = { ID_HELLO_MESSAGE, "Hello from the server" };
			//	else
			//		ms = { ID_HELLO_MESSAGE, "Hello from the client" };
			//	peer->Send((char*)&ms, sizeof(ms), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			//}
			//break;
				*/
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}