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

	strcpy(myUsername, "SERVERHOSTADMIN");

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
		strcpy(myUsername, "SERVERHOSTADMIN");

		printf("\nYour IPV4 Address is: %s \n", peer->GetLocalIP(0));
		strcpy(mData.promptBuffer, "Welcome to the Buckroom!\nLet's talk Toronto\n");

		infoSet = 1;
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

				time_t t = time(0);
				tm* timedata;
				timedata = std::localtime(&t);


				// reference on how to format time stamp: https://stackoverflow.com/questions/5438482/getting-the-current-time-as-a-yyyy-mm-dd-hh-mm-ss-string
				char timeString[64];
				std::strftime(timeString, 80, "%Y-%m-%d-%H-%M-%S", timedata);
				std::puts(timeString);

				char str[STR_MAX];

				sprintf(str, "Client %s assigned ID #%i at %s\n", pmsIn->username, 0, timeString);

				std::ofstream out("names.txt", std::ofstream::out);

				out << str;

				out.close();

				printf(str);

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

						time_t t = time(0);
						tm* timedata;
						timedata = std::localtime(&t);


						// reference on how to format time stamp: https://stackoverflow.com/questions/5438482/getting-the-current-time-as-a-yyyy-mm-dd-hh-mm-ss-string
						char timeString[64];
						std::strftime(timeString, 80, "%Y-%m-%d-%H-%M-%S", timedata);
						std::puts(timeString);

						char str[STR_MAX];

						sprintf(str, "Client %s assigned ID #%i at %s\n", pmsIn->username, i, timeString);

						std::ofstream out("names.txt", std::ios_base::app);

						out << str;

						out.close();

						printf(str);

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
		//----------------------------------------------------------------------------------------------------------------------------------------------------------	

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
			
			// if sent from server
			if (pmsIn->uniqueID == -1)
			{
				strcpy(broadcast.username, myUsername);
				// it came from us as the server
				if (pmsIn->destination[0] == '\0')
				{
					broadcast.isWhisper = 0;
					

					printf("%s: %s \n", myUsername, pmsIn->message);

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
					printf("%s Whispered to %s: %s \n", myUsername, pmsIn->destination, pmsIn->message);
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
			else
			{
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

			
		}

		//----------------------------------------------------------------------------------------------------------------------------------------------------------	


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

		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
	}
}

// Process data currently in the input buffer
void ServerState::processBuffer()
{
	// assign necessary rules/parameters
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
			system("cls");


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
	//ClientState::processBuffer();
	else if (infoSet == 1)
	{
		if (mData.buffer[0] == '\0')
			return;
		//Display the help function when the user input /h
		else if (mData.buffer[0] == '/' && mData.buffer[1] == 'H')
		{
			printf("\n/H prints all commands\n");
			printf("/E exits the server\n");
			printf("/W [USERNAME] sends a whisper\n");
			printf("Type normally to send a public message\n");
			return;
		}
		//Exit back to lobby if the user inputs /e
		else if (mData.buffer[0] == '/' && mData.buffer[1] == 'E')
		{
			//Close the connection from the peer
			peer->CloseConnection(peer->GetSystemAddressFromIndex(0), true);
			GoToNextState(mPrev); //Go to the lobby state

								  //Reset flags and clear buffers
			infoSet = 0;
			mData.doesDisplay = 1;
			clearBuffer();
			mData.port = 0;
			ipSet = 0;
			displayStrings.erase(displayStrings.begin(), displayStrings.end());
		}


		ClientChatMessage message;

		//Setting the message id to the client chat message ID
		message.messageID = ID_CLIENT_CHAT_MESSAGE;
		message.uniqueID = -1;	//Setting unique ID to the clients ID

								// Whisper to specific user
		if (mData.buffer[0] == '/' && mData.buffer[1] == 'W')
		{
			std::string tmp;
			unsigned int i;
			for (i = 3; i < mData.bufferIndex && mData.buffer[i] != ' '; ++i)
			{
				tmp += mData.buffer[i];
			}

			strcpy(message.destination, tmp.c_str());

			if (++i < mData.bufferIndex)
			{
				tmp = "";

				for (i; i < mData.bufferIndex; ++i)
				{
					tmp += mData.buffer[i];
				}

				strcpy(message.message, tmp.c_str());
			}
			else //Empty message, do not send
			{
				return;
			}
		}
		else if (mData.buffer == '\0')
		{
			return;
		}
		else //regular public message
		{
			strcpy(message.destination, "");
			strcpy(message.message, mData.buffer);
		}

		//As long as we are only recieving packets from the server this should work?
		peer->Send((char*)&message, sizeof(ClientChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::SystemAddress::SystemAddress("127.0.0.1", mData.port), false);

		clearBuffer();
		render();
	}
}

void ServerState::ArriveFromPreviousState(StateData * data)
{
	strcpy(mData.promptBuffer, "Please the port for your server: \n");
	mData.doesDisplay = 1;
}
