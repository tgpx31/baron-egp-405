#include "ServerState.h"

void ServerState::init(State * prev, State * nextL, State * nextR, State ** currentState)
{
	State::init(prev, nextL, nextR, currentState);
	mData.doesUpdateInput = 1;
	mData.doesUpdateNetworking = 1;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 0;

	mDataBase.connectedClientCount = 0;
	maxClients = 0;
	isServer = 0;

	peer = RakNet::RakPeerInterface::GetInstance();
	strcpy(mData.promptBuffer, "Please enter port number: \n");
}

// Receive and process incoming information from the network
void ServerState::updateNetworking()
{
	if (mData.port != 0 && maxClients > 0 && !isServer)
	{
		RakNet::SocketDescriptor sd(mData.port, 0);
		peer->Startup(maxClients, &sd, 1);
		peer->SetMaximumIncomingConnections(maxClients);
		isServer = 1;
		mData.doesUpdateInput = 0;
	}
	// message loop
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		switch (packet->data[0])
		{
			// Client connected messages
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
		{
			printf("Another client has connected.\n");
			/*++mDataBase.connectedClientCount;
			printf("Clients Connected: %i of max (%i)\n", mDataBase.connectedClientCount, maxClients);*/
			UsernameMessage msOut;
			strcpy(msOut.username, "");
			msOut.messageID = ID_USERNAME;
			peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			break;
		}

		case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A connection is incoming.\n");
			/*++mDataBase.connectedClientCount;
			printf("Clients Connected: %i of max (%i)\n", mDataBase.connectedClientCount, maxClients);*/
			UsernameMessage msOut;
			strcpy(msOut.username, "");
			msOut.messageID = ID_USERNAME;
			peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			break;
		}
			

		case ID_NEW_CLIENT_JOIN:
			UsernameMessage *pmsIn;
			pmsIn = (UsernameMessage*)packet->data;
			printf("Client Request: Join with username \'%s\'\n", pmsIn->username);

			// assign an identifier to associate the client with
			// ex,	TGPx31 joins, id 1
			//		TGPx31 joins, id 2
			// same username, distinguishably different clients
			// check for open IDs, ascending

			/*add the client to the list of clients with their ID
			send them their ID*/

			// iterate through the list of clients for the first empty client identifier
			for (unsigned int i = 0; i < maxClients; ++i)
			{
				if (mDataBase.clientList[i] == "")
				{
					// found an empty spot
					// send them their id
					mDataBase.clientList[i] == pmsIn->username;
					ClientNumberMessage msOut = { ID_CLIENT_NUMBER, i };
					peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					// broadcast welcome
					ChatMessage broadcast;
					broadcast.messageID = ID_SEND_ALL;
					std::string message = mDataBase.clientList[i] + " joined the server\n";
					strcpy(broadcast.message, message.c_str());
					peer->Send((char*)&broadcast, sizeof(broadcast), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
					return;
				}
			}




			//for (unsigned int i = 0; i < maxClients; ++i)
			//{
			//	if (mDataBase.clientDictionary.find(i) == mDataBase.clientDictionary.end())	// if there isn't an entry with the ID
			//	{
			//		// assign the first open ID
			//		mDataBase.clientDictionary.insert(std::pair <int, std::string>(i, pmsIn->username));
			//		printf("Client %s assigned ID #%i\n", pmsIn->username, i);
			//		return;
			//	}
			//}

			++mDataBase.connectedClientCount;
			printf("Clients Connected: %i of max (%i)\n", mDataBase.connectedClientCount, maxClients);
			
			// broadcast welcome message to all connected clients


			break;

			// Other


			{
				
			}



			// Client lost messages
		case ID_CONNECTION_LOST:
			printf("A client lost the connection.\n");
			--mDataBase.connectedClientCount;
			// iterate through the map, find disconnected client and remove them

			printf("Clients Connected: %i of max (%i)\n", mDataBase.connectedClientCount, maxClients);
			break;

		case ID_DISCONNECTION_NOTIFICATION:
			printf("A client has disconnected.\n");
			--mDataBase.connectedClientCount;
			printf("Clients Connected: %i of max (%i)\n", mDataBase.connectedClientCount, maxClients);
			break;

		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
	}
}

// Process data currently in the input buffer
void ServerState::processBuffer()
{
	if (mData.port == 0)
	{
		sscanf(mData.buffer, "%i", &mData.port);
		// printf("\nPort Number: %i \n", mData.port);
		// Set the prompt to ask for max clients
		if (mData.port > 0)
			strcpy(mData.promptBuffer, "Please enter the maximum clients: \n");

		// Clear the buffer
		clearBuffer();
		render();
	}
	else if (mData.port != 0 && maxClients == 0)
	{
		sscanf(mData.buffer, "%i", &maxClients);

		if (maxClients > 0)
		{
			printf("\nPort Number: %i \n", mData.port);
			printf("\nMax Clients: %i \n", maxClients);
		}
		else
		{
			// Clear the buffer
			clearBuffer();
			render();
		}

		// Clear the buffer
		clearBuffer();
	}

	// Clear the buffer
	clearBuffer();
}

void ServerState::ArriveFromPreviousState(StateData * data)
{
	strcpy(mData.promptBuffer, "Please the port for your server: \n");
	mData.doesDisplay = 1;
}
