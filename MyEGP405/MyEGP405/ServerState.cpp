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

	mTimer.start();
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
		{
			UsernameMessage *pmsIn;

			pmsIn = (UsernameMessage*)packet->data;
			printf("Client Request: Join with username \'%s\'\n", pmsIn->username);

			if (mDataBase.connectedClientCount == 0)
			{
				// assign the first open ID
				int id = 0;
				ClientInfo ci;
				ci.address = packet->systemAddress;
				strcpy(ci.username, pmsIn->username);

				mDataBase.clientDictionary.insert(std::pair <int, ClientInfo>(0, ci));

				printf("Client %s assigned ID #%i at %d\n", pmsIn->username, 0, mTimer.getElapsedTime());

				// send them their id
				ClientNumberMessage msOut = { ID_CLIENT_NUMBER, 0 };
				peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				// broadcast welcome
				ClientChatMessage broadcast;
				broadcast.messageID = ID_SEND_ALL;
				std::string message = (std::string)pmsIn->username + " joined the server\n";
				strcpy(broadcast.message, message.c_str());

				for (unsigned int j = 0; j < maxClients; ++j)
				{
					//If this already exists, broadcast
					if (mDataBase.clientDictionary.count(j) > 0)
					{
						peer->Send((char*)&broadcast, sizeof(ClientChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mDataBase.clientDictionary[j].address, false);

					}
				}
			}
			else
			{
				for (unsigned int i = 0; i < maxClients; ++i)
				{
					if (mDataBase.clientDictionary.count(i) <= 0)	// if there isn't an entry with the ID
					{
						// assign the first open ID
						int id = i;
						ClientInfo ci;
						ci.address = packet->systemAddress;
						strcpy(ci.username, pmsIn->username);

						mDataBase.clientDictionary.insert(std::pair <int, ClientInfo>(i, ci));

						printf("Client %s assigned ID #%i\n", pmsIn->username, i);

						// send them their id
						ClientNumberMessage msOut = { ID_CLIENT_NUMBER, i };
						peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

						// broadcast welcome
						ClientChatMessage broadcast;
						broadcast.messageID = ID_SEND_ALL;
						std::string message = (std::string)pmsIn->username + " joined the server\n";
						strcpy(broadcast.message, message.c_str());

						for (unsigned int j = 0; j < maxClients; ++j)
						{
							//If this already exists, broadcast
							if (mDataBase.clientDictionary.count(j) > 0)
							{
								peer->Send((char*)&broadcast, sizeof(ClientChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mDataBase.clientDictionary[j].address, false);

							}
						}
						break;
					}
				}
			}

			++mDataBase.connectedClientCount;
			printf("Clients Connected: %i of max (%i)\n", mDataBase.connectedClientCount, maxClients);
			break;
		}
			

		case ID_CLIENT_CHAT_MESSAGE:
		{
			ClientChatMessage* pmsIn;
			pmsIn = (ClientChatMessage*)packet->data;

			std::string username = mDataBase.clientDictionary[pmsIn->uniqueID].username;
			std::string message = pmsIn->message;
	
			// broadcast welcome
			ServerChatMessage broadcast;
			broadcast.messageID = ID_SERVER_CHAT_MESSAGE;
			strcpy(broadcast.username, username.c_str());
			strcpy(broadcast.message, message.c_str());
			
			// Public messaage
			if (pmsIn->destination[0] == '\0')
			{
				broadcast.isWhisper = 0;
				printf("%s: %s \n", mDataBase.clientDictionary[pmsIn->uniqueID].username, pmsIn->message);

				for (unsigned int j = 0; j < maxClients; ++j)
				{
					//If this already exists, broadcast
					if (mDataBase.clientDictionary.count(j) > 0)
					{
						peer->Send((char*)&broadcast, sizeof(ServerChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mDataBase.clientDictionary[j].address, false);
					}
				}
			}
			else // Whisper message
			{
				broadcast.isWhisper = 1;
				printf("%s Whispered to %s: %s \n", mDataBase.clientDictionary[pmsIn->uniqueID].username, pmsIn->destination, pmsIn->message);
				for (std::map<int, ClientInfo>::iterator it = mDataBase.clientDictionary.begin(); it != mDataBase.clientDictionary.end(); ++it)
				{
					if (strcmp(it->second.username, pmsIn->destination) == 0)
					{
						peer->Send((char*)&broadcast, sizeof(ServerChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, it->second.address, false);
					}
				}
			}
			break;

			break;
		}



			// Client lost messages
		case ID_CONNECTION_LOST:
			printf("A client lost the connection.\n");
			--mDataBase.connectedClientCount;
			if (mDataBase.connectedClientCount == 0)
				return;
			// iterate through the map, find disconnected client and remove them
			for (int i = 0; i < maxClients; ++i)
			{
				//I think that'll work - Brian
				if (mDataBase.clientDictionary.at(i).address == packet->systemAddress)
				{
					mDataBase.clientDictionary.erase(i);
					return;
				}
			}

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
