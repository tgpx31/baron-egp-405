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
#include "ClientState.h"
#include <iostream>
#include <string>

// Receive and process incoming information from the network
void ClientState::updateNetworking()
{
	//Attempt to connect if IP and the port number are set
	if (ipSet == 1 && mData.port != 0 && !requestConnection)
	{
		peer->Connect(mData.connectionAddress, mData.port, 0, 0); //Peer connection at the connection address and the port
		printf("Requesting connection...\n");

		strcpy(mData.promptBuffer, "Welcome to the Buckroom!\nLet's talk Toronto\n");

		requestConnection = 1;
		infoSet = 1;
	}
	else if (requestConnection) // Else if the connection has been requested
	{
		// Message Loop
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
				// Client connected messages
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\nSending our username to the server...\n");
				break;
			}
			case ID_USERNAME:
			{
				// Once connected, send message w/ user data to prompt welcome broadcast
				UsernameMessage msOut;
				strcpy(msOut.username, myUsername);
				msOut.messageID = ID_NEW_CLIENT_JOIN;

				peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				break;
			}
			case ID_CLIENT_NUMBER:
			{
				//Send message w/ the clients ID# 
				ClientNumberMessage* pmsIn;
				pmsIn = (ClientNumberMessage*)packet->data;
				id = pmsIn->clientNumber;
				printf("We are assigned to ID#%i\n", id);
				break;
			}
			case ID_SERVER_CHAT_MESSAGE:	//Case for the client sending a chat message
			{
				//Create the server chat message
				ServerChatMessage* pmsIn;
				pmsIn = (ServerChatMessage*)packet->data;
				
				//If the user is seeing their own message, do not change color of the window for notification
				if (strcmp(pmsIn->username, myUsername))
					system("Color 1A");

				char tmpChar[555];

				//If the message is a whisper
				if (pmsIn->isWhisper)
				{
					system("Color 3B"); //Change the color
					sprintf(tmpChar, "%s whispered: %s\n", pmsIn->username, pmsIn->message); //Display who wispered to you followed by the message
				}
				else //Else its a normal message
				{
					sprintf(tmpChar, "%s: %s\n", pmsIn->username, pmsIn->message); //Display the user and message they sent
				}
				
				printf("%s", tmpChar);

				std::string newString(tmpChar);

				displayStrings.push_back(newString);

				break;
			}
			// Other
			
			// Broadcast message recieve
			case ID_SEND_ALL:
			{
				ClientChatMessage *pmsIn;
				pmsIn = (ClientChatMessage*)packet->data;
				printf("TO_ALL:\t%s\n", pmsIn->message);
				break;
			}
			
			// Full Server
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;

				// Client lost messages
			case ID_DISCONNECTION_NOTIFICATION:
				printf("\tWe have been disconnected.\n");
				break;

			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
				break;


			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}
}

// Process data currently in the input buffer
void ClientState::processBuffer()
{
	//If the client info hasn't yet been set, get it
	if (infoSet == 0)
	{
		getClientInfo();
	}
	//Else get chat input
	else if(infoSet == 1)
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
		message.uniqueID = id;	//Setting unique ID to the clients ID

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
		peer->Send((char*)&message, sizeof(ClientChatMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);

		clearBuffer();
		render();
	}
}

void ClientState::getClientInfo()
{
	//If the connection address is still the default
	if (strcmp(mData.connectionAddress, "default\n") && ipSet == 0)
	{
		if (mData.buffer[0] == '\0')
			strcpy(mData.connectionAddress, "127.0.0.1");
		else //Else copy the inputted IP to the connection Address
			strcpy(mData.connectionAddress, mData.buffer);

		//Set the prompt to ask for the port number
		strcpy(mData.promptBuffer, "Please enter server port number: \n");

		// Clear the buffer
		clearBuffer();
		render();

		ipSet = 1;
	}
	//Else if the port number is still the default value
	else if (mData.port == 0 && ipSet == 1)
	{
		sscanf(mData.buffer, "%i", &mData.port);
		printf("\nIP Address: %s \n", mData.connectionAddress);
		printf("\nPort Number: %i \n", mData.port);

		//Prompt for a user 
		strcpy(mData.promptBuffer, "Please enter desired user name: \n");

		clearBuffer();
		render();
	}
	//Have the client set their user name
	else if (mData.port != 0 && ipSet == 1)
	{
		//If they inputted nothing use a default user name
		if (mData.buffer[0] == '\0')
			strcpy(myUsername, "GUESTUSER");
		else //Else copy the inputted user name into the username slot
			strcpy(myUsername, mData.buffer);
		printf("\nUser Name: %s \n", myUsername);

		requestConnection = 0;		
	}

	// Clear the buffer
	clearBuffer();
}

void ClientState::updateData()
{
	int i;

	// Number keys
	for (i = 0x30; i < 0x3A; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	// Numpad
	for (i = 0x60; i < 0x6A; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	// Decimal key/period
	i = 0x6E;
	if (mData.keyboard[0x6E] || mData.keyboard[0xBE] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	//Disable spaces for the username so that we can have PM's work correctly
	if (mData.keyboard[VK_SPACE] && mData.bufferIndex < STR_MAX && infoSet)
	{
		mData.buffer[mData.bufferIndex] = ' ';
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	if (mData.keyboard[VK_OEM_2] && mData.bufferIndex < STR_MAX)
	{
		mData.buffer[mData.bufferIndex] = '/';
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	// Alphabet
	for (i = 0x41; i < 0x5B; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < STR_MAX)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	if (mData.keyboard[VK_BACK])
	{
		if (mData.bufferIndex == 0)
			return;
		mData.buffer[--mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}



	if (mData.keyboard[VK_RETURN])
	{
		processBuffer();
		clearBuffer();
	}
}

//Arriving from the previous state
void ClientState::ArriveFromPreviousState(StateData *data)
{
	strcpy(mData.promptBuffer, "Please enter server IP: \n");
	mData.doesDisplay = 1;
}

//Initializing the client state
void ClientState::init(State* prev, State* nextL, State* nextR, State** currentState)
{
	State::init(prev, nextL, nextR, currentState);

	strcpy(myUsername, "GUESTUSER");
	strcpy(mData.promptBuffer, "Please enter server IP: \n");

	//Setting flags
	mData.doesUpdateInput = 1;
	mData.doesUpdateNetworking = 1;
	mData.doesUpdateState = 1;
	mData.doesDisplay = 0;

	ipSet = 0;
	requestConnection = 1;
	id = 0;
	infoSet = 0;

	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);
}

void ClientState::render()
{
	//Set background color
	system("Color 0F");
	system("CLS");
	std::cout << mData.promptBuffer;
	
	for (int i = 0; i < displayStrings.size(); ++i)
	{
		std::cout << displayStrings[i];
	}

	std::cout << mData.buffer;

	mData.doesDisplay = 0;
}
