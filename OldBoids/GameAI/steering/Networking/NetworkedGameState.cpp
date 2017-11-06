// Brian Baron		0974390
// Justin Mulkin	0952465
//
// EGP 405-02	Lab 2	2017/09/25	(YYYY/MM/DD)
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

// This file wasmodified by Brian Baron, Colin Brady, and Justin Mulkin with permission from author.

#include "NetworkedGameState.h"
#include "Messages.h"

void NetworkedGameState::ArriveFromPreviousState(StateData * data)
{
	GameState::ArriveFromPreviousState(data);

	if (data != nullptr)
	{
		// Are you a host or not?
		mData.mIsHost = data->mIsHost;
		mData.dataMethod = data->dataMethod;
	}

	mData.doesDisplay = 1;

	if (mData.mIsHost)
	{
		RakNet::SocketDescriptor sd(DEFAULT_PORT_NUMBER, 0);
		peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
		peer->SetMaximumIncomingConnections(MAX_PEER_CONNECTIONS);
		willSendState = 1; //Data coupled - For if it will be sending the state
		isX = 0;
	}
	else
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
		peer->Connect("127.0.0.1", DEFAULT_PORT_NUMBER, 0, 0);
		willSendState = 0; //Data coupled
		isX = 1;
	}

	connectionSet = 1;
	networkingSetup = 1;
	//Set it to update the networking
	mData.doesUpdateNetworking = 1;

	//GraphicsBuffer* pAIBuffer = mGameState.mpGraphicsBufferManager->getBuffer(mGameState.mEnemyIconBufferID);
	remotePlayerSprite = mGameState.mpSpriteManager->getSprite(2);
	//remotePlayerSprite = mGameState.mpSpriteManager->createAndManageSprite(2, mGameState.mpGraphicsBufferManager->getBuffer(mGameState.mEnemyIconBufferID), 0, 0, mGameState.mpGraphicsBufferManager->getBuffer(mGameState.mEnemyIconBufferID)->getWidth(), mGameState.mpGraphicsBufferManager->getBuffer(mGameState.mEnemyIconBufferID)->getHeight());
	//remotePlayerSprite = mGameState.mpSpriteManager->createAndManageSprite(2, pAIBuffer, 0, 0, pAIBuffer->getWidth(), pAIBuffer->getHeight());
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
	strcpy(mData.promptBuffer, "Boids Online: Hosting\nMake sure to give your friend your IP address!\n(You can get your IP by opening a cmd and entering ipconfig)\nAwaiting connection...\n");

	eventQueue = DeanQueue();
	eventQueue.Clear();
}

int NetworkedGameState::SerializePlayer(char * buffer)
{
	int bytesWritten = 0;
	int size = 1;

	// pass number of players
	bytesWritten += Write(buffer, &size);

	//for each (Player* player in otherPlayers)
	{
		// Save off info
		float posX = mGameState.mpLocalPlayer->getPosition().getX();
		float posY = mGameState.mpLocalPlayer->getPosition().getY();

		bytesWritten += Write(buffer, &posX);
		bytesWritten += Write(buffer, &posY);
	}

	return bytesWritten;
}

void NetworkedGameState::DeserializePlayers(char * buffer)
{
	// avoid the identifier
	++buffer;

	//*(int*)buffer
	int remotePlayerCount = 1, prevRemoteCount = otherPlayers.size();
	buffer += sizeof(int);

	// check num players recieved
	if (remotePlayerCount > prevRemoteCount)
	{
		// more players, add to list
		for (; prevRemoteCount < remotePlayerCount; ++prevRemoteCount)
		{
			Sprite* tmp = mGameState.mpSpriteManager->getSprite(1);
			Player* newPlayer = new Player(tmp, gZeroVector2D, 0, 1);
			otherPlayers.push_back(newPlayer);
		}
	}
	else if (remotePlayerCount < prevRemoteCount)
	{
		// less players, remove from list
		for (; prevRemoteCount > remotePlayerCount; --prevRemoteCount)
		{
			//delete otherPlayers[prevRemoteCount];
			otherPlayers.pop_back();
		}
	}

	for each (Player* player in otherPlayers)
	{
		float posX, posY;

		memcpy(&posX, buffer, sizeof(float));
		buffer += sizeof(float);
		memcpy(&posY, buffer, sizeof(float));
		buffer += sizeof(float);

		player->setPosition(posX, posY);
		player->setSprite(remotePlayerSprite);
	}
}

//****TODO: Add event queue system
void NetworkedGameState::updateInput()
{
	//Get Input
	al_get_mouse_state(&mGameState.mCurrentMouseState);
	al_get_keyboard_state(&mGameState.mCurrentKeyboardState);

	// Exit
	if (keyPressed(ALLEGRO_KEY_ESCAPE))
	{
		// Stop updating
		mData.doesUpdateInput = mData.doesUpdateState = mData.doesUpdateNetworking = mData.doesDisplay = 0;

		// Cleanup
		cleanup();

		// Exit state to lobby
		// for now exit everything
		mData.running = 0;
		return;
	}

	// Movement
	if (keyPressed(ALLEGRO_KEY_A))
	{
		// walk left
		//mGameState.mpLocalPlayer->setXVelocity(-1);
		//****TODO: have this event sent out to connected other, and do the local here
		MovePlayerMessage message;
		message.ID = ID_MOVE_PLAYER;
		message.xVel = -1, message.yVel = 0;
		message.playerID = isX;

		peer->Send((char*)&message, sizeof(message), HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
	}
	else if (keyUp(ALLEGRO_KEY_A))
	{
		//mGameState.mpLocalPlayer->setXVelocity(0);
		MovePlayerMessage message;
		message.ID = ID_MOVE_PLAYER;
		message.xVel = 0, message.yVel = 0;
		message.playerID = isX;

		peer->Send((char*)&message, sizeof(message), HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
	}

	if (keyPressed(ALLEGRO_KEY_D))
	{
		// walk right
		//mGameState.mpLocalPlayer->setXVelocity(+1);
		MovePlayerMessage message;
		message.ID = ID_MOVE_PLAYER;
		message.xVel = +1, message.yVel = 0;
		message.playerID = isX;

		peer->Send((char*)&message, sizeof(message), HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
	}
	else if (keyUp(ALLEGRO_KEY_D))
	{
		//mGameState.mpLocalPlayer->setXVelocity(0);
		MovePlayerMessage message;
		message.ID = ID_MOVE_PLAYER;
		message.xVel = 0, message.yVel = 0;
		message.playerID = isX;

		peer->Send((char*)&message, sizeof(message), HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
	}

	mGameState.mPreviousKeyboardState = mGameState.mCurrentKeyboardState;
	mGameState.mPreviousMouseState = mGameState.mCurrentMouseState;
}

void NetworkedGameState::updateData()
{
	//Update the state and gamestate data
	if (!connectionSet)
		State::updateData();
	else if (connectionSet)
	{
		// base update
		// updates your local player right now
		GameState::updateData();

		// clients in data push mode don't send packets, both data sharing peers send packets
		if ((mData.dataMethod == 1 && mData.mIsHost) || mData.dataMethod == 2)
		{
			char buffer[1024];
			int bytesWritten = 0;
			//****TODO: Serialize data and send it

			buffer[0] = ID_PLAYERS_DATA;
			++bytesWritten;
			bytesWritten += SerializePlayer(buffer + bytesWritten);
			if (bytesWritten >= 5) //If it's not just ID plus number of list size
				peer->Send(buffer, bytesWritten, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
		}

		eventQueue.ExecuteAllEvents();
	}
}

void NetworkedGameState::updateNetworking()
{
	if (!networkingSetup)
		return;


	// message loop
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_PLAYERS_DATA:
			DeserializePlayers((char*)packet->data);
			break;

		case ID_MOVE_PLAYER:
		{
			MovePlayerMessage *pMsg = (MovePlayerMessage*)packet->data;

			// get player obj associated with id
			if (!pMsg->playerID == isX)
			{
				MovePlayerEvent *moveEv = new MovePlayerEvent(ID_MOVE_PLAYER, Vector2D(pMsg->xVel, pMsg->yVel), mGameState.mpLocalPlayer, 0);
				eventQueue.Push(moveEv);
				std::cout << "Received Move Player Message";
			}

			break;
		}
		}
	}
}


//****TODO: Update this to accomodate networked stuff
void NetworkedGameState::render()
{
	if (!initialized)
		return;

	// draw bg
	Sprite* pBackgroundSprite = mGameState.mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	pBackgroundSprite->draw(*(mGameState.mpGraphicsSystem->getBackBuffer()), 0, 0);

	// draw your player
	mGameState.mpLocalPlayer->draw(mGameState.mpGraphicsSystem->getBackBuffer());

	/*for each (Player* player in otherPlayers)
	{
		player->draw(mGameState.mpGraphicsSystem->getBackBuffer());
	}*/
	if (otherPlayers.size() >= 1)
	{
		otherPlayers.at(0)->draw(mGameState.mpGraphicsSystem->getBackBuffer());

	}

	// flip the buffers
	mGameState.mpGraphicsSystem->swap();
}
