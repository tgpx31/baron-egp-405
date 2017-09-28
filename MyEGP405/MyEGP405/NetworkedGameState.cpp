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
		RakNet::SocketDescriptor sd;// (DEFAULT_PORT_NUMBER, 0);
		peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);

		//Setting the isX flag to one to indicate which letter the client is
		isX = 0;
	}
	else if (mData.mIsHost)
	{
		//If you are the host:
		RakNet::SocketDescriptor sd(DEFAULT_PORT_NUMBER, 0);

		//Startup the peer
		peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
		peer->SetMaximumIncomingConnections(MAX_PEER_CONNECTIONS);

		connectionSet = 1;
		networkingSetup = 1;

		//Setting the isX flag to one to indicate which letter the host is
		isX = 1;
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
	isPlaying = 0;

	//Default prompt for the host
	strcpy(mData.promptBuffer, "Tic-Tac-Toe Online: Hosting\nMake sure to give your friend your IP address!\n(You can get your IP by opening a cmd and entering ipconfig)\nAwaiting connection...\n");
}

void NetworkedGameState::updateData()
{
	//Update the state and gamestate data
	if (!connectionSet)
		State::updateData();
	else if (connectionSet)
	{
		//If the player priority is equal to the character flag, let the player have a turn
		if (isX == mGameStateData.playerPriority)
		{
			GameState::updateData();
		}
	}
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
		peer->Connect(connectionAddress, DEFAULT_PORT_NUMBER, 0, 0);
		printf("Requesting connection...\n");

		networkingSetup = 1;
	}

	if (!networkingSetup)
		return;

	// message loop
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		switch (packet->data[0])
		{

		// New connections
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			printf("Our connection has been accepted.\n");
			isPlaying = 1;
			// SETUP GAME BOARD
			strcpy(mData.promptBuffer, "* Tic-Tac-Toe * Use WASD to move your selection, press ENTER to confirm *\n*************************************************************************\n");
			
			render();

			// SYNC WHOSE TURN IT IS
			break;
		}

		case ID_REMOTE_NEW_INCOMING_CONNECTION:
		{
			printf("Another client has connected.\n");
			break;
		}

		case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A connection is incoming.\n");
			isPlaying = 1;
			// SETUP GAME BOARD
			strcpy(mData.promptBuffer, "* Tic-Tac-Toe * Use WASD to move your selection, press ENTER to confirm *\n*************************************************************************\n");

			render();

			// SYNC WHOSE TURN IT IS
			break;
		}

		// Connection Lost
		case ID_CONNECTION_LOST:
		{
			printf("A client lost the connection.\n");
			// become host, awaiting friend to join
			//mData.mIsHost = 1;
			//// close the connection
			//peer->Shutdown(0);

			////Setting flags
			//connectionSet = 0;
			//networkingSetup = 0;

			////Default prompt for the host
			//strcpy(mData.promptBuffer, "Tic-Tac-Toe Online: Hosting\nMake sure to give your friend your IP address!\n(You can get your IP by opening a cmd and entering ipconfig)\nAwaiting connection...\n");

			//RakNet::SocketDescriptor sd(DEFAULT_PORT_NUMBER, 0);

			////Startup the peer
			//peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
			//peer->SetMaximumIncomingConnections(MAX_PEER_CONNECTIONS);

			//connectionSet = 1;
			//networkingSetup = 1;
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
	if (!isPlaying)
		State::render();
	else if (isPlaying)
		GameState::render();
}