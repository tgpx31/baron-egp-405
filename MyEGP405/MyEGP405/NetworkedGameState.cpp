#include "NetworkedGameState.h"
#include "Messages.h"

void NetworkedGameState::ArriveFromPreviousState(StateData * data)
{
	if (data != nullptr)
	{
		// Are you a host or not?
		mData.mIsHost = data->mIsHost;
	}

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
		if (isX == mGameStateData.playerPriority || mGameStateData.endGame)
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
	//Else if the connection has been set and you are in game
	else if (isPlaying)
	{
		if (!mGameStateData.endGame)
		{
			switch (mData.buffer[0])
			{
				// WASD: move the 'selected space'
				// **** TODO:
				// make sure the next selected space 
			case 'W':
			{
				if (mGameStateData.selectedSpace >= 3)
				{
					// reset the tmpBoard to the current board
					strcpy(mGameStateData.tmpBoard, mGameStateData.board);

					mGameStateData.selectedSpace -= 3;

					// Change the char to the 'select char'
					*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
					//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
				}
				break;
			}
			case 'A':
			{
				if (mGameStateData.selectedSpace >= 1 &&
					mGameStateData.selectedSpace != 3 &&
					mGameStateData.selectedSpace != 6)
				{
					// reset the tmpBoard to the current board
					strcpy(mGameStateData.tmpBoard, mGameStateData.board);

					mGameStateData.selectedSpace -= 1;

					// Change the char to the 'select char'
					*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
					//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
				}
				break;
			}
			case 'S':
			{
				if (mGameStateData.selectedSpace <= 5)
				{
					// reset the tmpBoard to the current board
					strcpy(mGameStateData.tmpBoard, mGameStateData.board);

					mGameStateData.selectedSpace += 3;

					// Change the char to the 'select char'
					*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
					//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
				}
				break;
			}
			case 'D':
			{
				if (mGameStateData.selectedSpace <= 7 &&
					mGameStateData.selectedSpace != 2 &&
					mGameStateData.selectedSpace != 5)
				{
					// reset the tmpBoard to the current board
					strcpy(mGameStateData.tmpBoard, mGameStateData.board);

					mGameStateData.selectedSpace += 1;

					// Change the char to the 'select char'
					*(mGameStateData.tmpBoard + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = '@';
					//printf("\nSelected space %i\n", mGameStateData.selectedSpace);
				}
				break;
			}

			// ESC
			case '\x1b':
			{
				printf("ESC pressed\n");
				// Active player forfeits
				mGameStateData.endGame = 1;
				mGameStateData.winner = 1 - mGameStateData.playerPriority;

				strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");

				mData.doesDisplay = 1;
				break;
			}

			// Enter: Check if valid space
			case '\0':
				if (validateMove())
				{
					//If the move is valid, send the move message
					GameMoveMessage msgOut;
					msgOut.id = ID_VALID_MOVE;
					msgOut.placementIndex = mGameStateData.selectedSpace;

					//Send based on address saved when connecting			
					peer->Send((char*)&msgOut, sizeof(msgOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);

					// if valid move, update the board
					// set the selected space to your char
					mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
					*(mGameStateData.board + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = mGameStateData.currentPlayerChar;

					// check if player won
					if (checkWin())
					{
						mGameStateData.endGame = 1;
						mGameStateData.winner = mGameStateData.playerPriority;

						strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");
					}
					// check if draw
					else if (checkDraw())
					{
						mGameStateData.endGame = 1;
						mGameStateData.winner = -1;

						strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");
					}
					// if not, switch player priority
					else
					{
						mGameStateData.playerPriority = 1 - mGameStateData.playerPriority;
						mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
					}

					// reset selected space
					mGameStateData.selectedSpace = 4;

					// Display
					mData.doesDisplay = 1;
				}
				else
				{
					printf("\n\tERROR: That space is taken!\n");
				}
				break;

			default:
				printf("\n\tERROR: Invalid Input\n");
				break;

			}
		}
		else if (mGameStateData.endGame)
		{
			switch (mData.buffer[0])
			{
			case 'R':
			{
				strcpy(mData.promptBuffer, "* Tic-Tac-Toe * Use WASD to move your selection, press ENTER to confirm *\n*************************************************************************\n");
				strcpy(mGameStateData.board, "  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n-----------------\n  _  |  _  |  _  \n");
				strcpy(mGameStateData.tmpBoard, mGameStateData.board);
				//mGameStateData.tmpBoard[mGameStateData.boardSpaceOffsets[4]] = '@';

				// flip the starting player flag
				mGameStateData.initialPlayerPriority = 1 - mGameStateData.initialPlayerPriority;
				mGameStateData.playerPriority = mGameStateData.initialPlayerPriority;
				mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';

				mGameStateData.winner = -1;
				mGameStateData.endGame = 0;

				mData.doesDisplay = 1;
				break;
			}

			case 'M':
			{
				//Send a peer has left message
				GameMoveMessage msgOut;
				msgOut.id = ID_PEER_LEFT;
				msgOut.placementIndex = 0;

				//Send based on address saved when connecting			
				peer->Send((char*)&msgOut, sizeof(msgOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);

				//Close the connections
				//peer->CloseConnection(address, false, RELIABLE_ORDERED, HIGH_PRIORITY);
				//Init the state
				peer->Shutdown(1, RELIABLE_ORDERED, HIGH_PRIORITY);
				init(nullptr, nullptr);
				
				GoToNextState(mGameStateData.mPrev);
				return;
				break;
			}

			default:
				printf("\n\tERROR: Invalid Input\n");
				break;
			}
		}
		// Clear the buffer
		clearBuffer();
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

			//Get a permanent reference to your peer
			address = packet->systemAddress;
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

			//Get a permanent reference to your peer
			address = packet->systemAddress;
			// SETUP GAME BOARD
			strcpy(mData.promptBuffer, "* Tic-Tac-Toe * Use WASD to move your selection, press ENTER to confirm *\n*************************************************************************\n");

			render();

			// SYNC WHOSE TURN IT IS
			break;
		}

		case ID_DISCONNECTION_NOTIFICATION:
		{
			printf("A client has disconnected.\n");
			break;
		}

		//Case for receiving a move message
		case ID_VALID_MOVE:
		{
			//Create a gamemovemessage to recieve data from
			GameMoveMessage* msgIn;
			msgIn = (GameMoveMessage*)packet->data;

			//Get the placement index from the packet
			mGameStateData.selectedSpace = msgIn->placementIndex;

			// if valid move, update the board
			// set the selected space to your char
			mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
			*(mGameStateData.board + mGameStateData.boardSpaceOffsets[mGameStateData.selectedSpace]) = mGameStateData.currentPlayerChar;

			// set the current board
			//strcpy(mGameStateData.board, mGameStateData.tmpBoard);

			// check if player won
			if (checkWin())
			{
				mGameStateData.endGame = 1;
				mGameStateData.winner = mGameStateData.playerPriority;

				strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");
			}
			// check if draw
			else if (checkDraw())
			{
				mGameStateData.endGame = 1;
				mGameStateData.winner = -1;

				strcpy(mData.promptBuffer, "(R)ematch or (M)enu?\n");
			}
			// if not, switch player priority
			else
			{
				mGameStateData.playerPriority = 1 - mGameStateData.playerPriority;
				mGameStateData.currentPlayerChar = mGameStateData.playerPriority ? 'X' : 'O';
			}

			// reset selected space
			mGameStateData.selectedSpace = 4;

			strcpy(mGameStateData.tmpBoard, mGameStateData.board);

			// Display
			mData.doesDisplay = 1;

			break;
		}
		//Case for if one of the peers has left
		case ID_PEER_LEFT:
		{
			//Close the connections
			//peer->CloseConnection(address, false, RELIABLE_ORDERED, HIGH_PRIORITY);

			peer->Shutdown(1, RELIABLE_ORDERED, HIGH_PRIORITY);
			init(nullptr, nullptr);
			
			mData.mIsHost = true;
			ArriveFromPreviousState(nullptr);
			return;
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