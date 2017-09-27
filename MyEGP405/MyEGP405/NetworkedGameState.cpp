#include "NetworkedGameState.h"
#include "Messages.h"

void NetworkedGameState::ArriveFromPreviousState(StateData * data)
{
	// Are you a host or not?
	mData.mIsHost = data->mIsHost;
	mData.doesDisplay = 1;

	if (!mData.mIsHost)
		strcpy(mData.promptBuffer, "Tic-Tac-Toe Online: Connecting\nEnter your friend's IP address, or hit ENTER to use '127.0.0.1'\n");
}

void NetworkedGameState::init(State * prev = nullptr, State ** currentState = nullptr)
{
	// initialize the game state
	GameState::init(prev, currentState);

	strcpy(mData.promptBuffer, "Tic-Tac-Toe Online: Hosting\nMake sure to give your friend your IP address!\n(You can get your IP by opening a cmd and entering ipconfig)\nAwaiting connection...\n");

	// initialize the networking params
	mData.doesUpdateNetworking = 1;
	peer = RakNet::RakPeerInterface::GetInstance();

	memset(connectionAddress, '\0', sizeof(connectionAddress));
	port = DEFAULT_PORT_NUMBER;

	connectionSet = 0;
	networkingSetup = 0;
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
	}
}

void NetworkedGameState::updateNetworking()
{
	if (!connectionSet)
	{
		if (mData.mIsHost)
		{
			// Set up your connection (host)
			RakNet::SocketDescriptor sd(port, 0);
			peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
			peer->SetMaximumIncomingConnections(MAX_PEER_CONNECTIONS);
			render();

			connectionSet = 1;
			mData.doesDisplay = 1;
		}
		else if (!mData.mIsHost && connectionAddress[0] != '\0')
		{
			// Client peer connection setup
			peer->Connect(connectionAddress, port, 0, 0);
			printf("\nRequesting connection to %s...\n", connectionAddress);

			connectionSet = 1;
			mData.doesDisplay = 1;
		}
	}
	else if (connectionSet)
	{
		// Start the networking loop
		// check if all the networking setup is done
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			if (!networkingSetup)
			{
				switch (packet->data[0])
				{
					case ID_NEW_INCOMING_CONNECTION: case ID_REMOTE_NEW_INCOMING_CONNECTION:
				{
					printf("A connection is incoming.\n");

					if (mData.mIsHost)
						networkingSetup = 1;
					break;
				}

				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					printf("You've connected.\n");

					if (!mData.mIsHost)
						networkingSetup = 1;
					break;
				}

				case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					printf("Theres no room to join!\n");
					GoToNextState(mGameStateData.mPrev);
					break;
				}
					

				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
				}
			}
			else if (networkingSetup)
			{

			}




			//if (!networkingSetup)
			//{
			//	if (mData.mIsHost)
			//	{
			//		// Wait for your friend to connect

			//		/*if (peer->NumberOfConnections() <= 0)
			//			return;
			//		else
			//		{
			//			printf("Your friend is here!\n");
			//			networkingSetup = 1;
			//		}*/
			//	}
			//	else if (!mData.mIsHost)
			//	{
			//		// Do we need anything here?
			//		// we're not set up until our connection is accepted
			//	}
			//}
		}
		// end messaging loop

	}
}

void NetworkedGameState::render()
{
	if (!connectionSet && !networkingSetup)
		State::render();
	else if (connectionSet && networkingSetup)
		GameState::render();
}