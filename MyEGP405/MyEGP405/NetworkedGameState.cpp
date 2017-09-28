#include "NetworkedGameState.h"
#include "Messages.h"

void NetworkedGameState::ArriveFromPreviousState(StateData * data)
{
	// Are you a host or not?
	mData.mIsHost = data->mIsHost;
	mData.doesDisplay = 1;

	if (!mData.mIsHost)
	{
		//Prompt for if it is not the host
		strcpy(mData.promptBuffer, "Tic-Tac-Toe Online: Connecting\nEnter your friend's IP address, or hit ENTER to use '127.0.0.1'\n");

		//Setup the socket descriptor
		RakNet::SocketDescriptor sd(port, 0);
		peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
	}
	else if (mData.mIsHost)
	{
		//If you are the host:
		RakNet::SocketDescriptor sd(port, 0);

		//Startup the peer
		peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
		peer->SetMaximumIncomingConnections(MAX_PEER_CONNECTIONS);

		connectionSet = 1;

		networkingSetup = 1;
	}

	//Set it to update the networking
	mData.doesUpdateNetworking = 1;

}

void NetworkedGameState::init(State * prev = nullptr, State ** currentState = nullptr)
{
	//Init the gamestate data
	GameState::init(prev, currentState);

	//Initialize the peer
	peer = RakNet::RakPeerInterface::GetInstance();

	//Initializing the connection address to the null terminator
	memset(connectionAddress, '\0', sizeof(connectionAddress));

	//Setting flags
	connectionSet = 0;
	networkingSetup = 0;

	//Default prompt for the host
	strcpy(mData.promptBuffer, "Tic-Tac-Toe Online: Hosting\nMake sure to give your friend your IP address!\n(You can get your IP by opening a cmd and entering ipconfig)\nAwaiting connection...\n");
}

void NetworkedGameState::updateData()
{
	if (!connectionSet)
		State::updateData();
	else if (connectionSet)
		GameState::updateData();
}

void NetworkedGameState::processBuffer()
{
	if (!connectionSet)
	{
		if (mData.buffer[0] == '\0')
			strcpy(connectionAddress, "127.0.0.1");
		else //Else copy the inputted IP to the connection Address
			strcpy(connectionAddress, mData.buffer);

		clearBuffer();

		connectionSet = 1;
	}
}

void NetworkedGameState::updateNetworking()
{
	//If you aren't a host and the connection is false
	if (connectionSet && !mData.mIsHost && !networkingSetup)
	{
		//Connect to the desired address and port
		peer->Connect(connectionAddress, port, 0, 0);

		printf("Requesting connection...\n");

		networkingSetup = 1;
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
			break;
		}

		case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A connection is incoming.\n");
			break;
		}
		// Client lost messages
		case ID_CONNECTION_LOST:
		{
			printf("A client lost the connection.\n");
			break;
		}

		case ID_DISCONNECTION_NOTIFICATION:
		{
			printf("A client has disconnected.\n");
			break;
		}

		default:
		{
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
		}
	}
}

void NetworkedGameState::render()
{
	if (!connectionSet && !networkingSetup)
		State::render();
	else if (connectionSet && networkingSetup)
		GameState::render();
}