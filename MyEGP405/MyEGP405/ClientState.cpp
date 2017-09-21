#include "ClientState.h"
#include <iostream>
#include <string>

// Receive and process incoming information from the network
void ClientState::updateNetworking()
{
	//Attempt to connect if IP and the port number are set
	if (ipSet == 1 && mData.port != 0 && !requestConnection)
	{
		peer->Connect(mData.connectionAddress, mData.port, 0, 0);
		printf("Requesting connection...\n");

		strcpy(mData.promptBuffer, "Welcome to the Buckroom!\nLet's talk Toronto\n");

		requestConnection = 1;
		infoSet = 1;
	}
	else if (requestConnection)
	{
		// **** TODO
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
				strcpy(msOut.username, username);
				msOut.messageID = ID_NEW_CLIENT_JOIN;

				peer->Send((char*)&msOut, sizeof(msOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				break;
			}
			case ID_CLIENT_NUMBER:
			{
				ClientNumberMessage* pmsIn;
				pmsIn = (ClientNumberMessage*)packet->data;
				id = pmsIn->clientNumber;
				printf("We are assigned to ID#%i\n", id);
				break;
			}
			case ID_SERVER_CHAT_MESSAGE:
			{
				ServerChatMessage* pmsIn;
				pmsIn = (ServerChatMessage*)packet->data;
				
				char tmpChar[555];
				sprintf(tmpChar, "%s: %s\n", pmsIn->username, pmsIn->message);
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
		ClientChatMessage message;

		//Setting the message id to the client chat message ID
		message.messageID = ID_CLIENT_CHAT_MESSAGE;
		message.uniqueID = id;	//Setting unique ID to the clients ID

		// Whisper to specific user
		if (mData.buffer[0] == '/' && mData.buffer[1] == 'W')
		{
			std::string tmp;
			int i;
			for (i = 0; i < mData.bufferIndex && mData.buffer[i] != ' '; ++i)
			{
				tmp += mData.buffer[i];
			}

			strcpy(message.destination, tmp.c_str());
			
			if (i > mData.bufferIndex)
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
			strcpy(username, "Guest_User");
		else //Else copy the inputted user name into the username slot
			strcpy(username, mData.buffer);
		printf("\nUser Name: %s \n", username);

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
		if (mData.keyboard[i] && mData.bufferIndex < 256)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	// Numpad
	for (i = 0x60; i < 0x6A; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < 256)
		{
			mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
			mData.buffer[++mData.bufferIndex] = '\0';
			mData.doesDisplay = 1;
		}
	}

	// Decimal key/period
	i = 0x6E;
	if (mData.keyboard[0x6E] || mData.keyboard[0xBE] && mData.bufferIndex < 256)
	{
		mData.buffer[mData.bufferIndex] = MapVirtualKey(i, MAPVK_VK_TO_CHAR);
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	//Disable spaces for the username so that we can have PM's work correctly
	if (mData.keyboard[VK_SPACE] && mData.bufferIndex < 256 && infoSet)
	{
		mData.buffer[mData.bufferIndex] = ' ';
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	if (mData.keyboard[VK_OEM_2] && mData.bufferIndex < 256)
	{
		mData.buffer[mData.bufferIndex] = '/';
		mData.buffer[++mData.bufferIndex] = '\0';
		mData.doesDisplay = 1;
	}

	// Alphabet
	for (i = 0x41; i < 0x5B; ++i)
	{
		if (mData.keyboard[i] && mData.bufferIndex < 256)
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

void ClientState::ArriveFromPreviousState(StateData *data)
{
	strcpy(mData.promptBuffer, "Please enter server IP: \n");
	mData.doesDisplay = 1;
}

//Initializing the client state
void ClientState::init(State* prev, State* nextL, State* nextR, State** currentState)
{
	State::init(prev, nextL, nextR, currentState);

	strcpy(username, "Guest_User");
	strcpy(mData.promptBuffer, "Please enter server IP: \n");

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
	system("CLS");
	std::cout << mData.promptBuffer;
	
	for (int i = 0; i < displayStrings.size(); ++i)
	{
		std::cout << displayStrings[i];
	}

	std::cout << mData.buffer;

	mData.doesDisplay = 0;
}
