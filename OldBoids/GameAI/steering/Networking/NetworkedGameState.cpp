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
#include "NetworkedGameState.h"
#include "Messages.h"
#include "../KinematicUnit.h"
#include <vector>

void NetworkedGameState::ArriveFromPreviousState(StateData * data)
{
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
	}
	else
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(MAX_PEER_CONNECTIONS, &sd, 1);
		peer->Connect("127.0.0.1", DEFAULT_PORT_NUMBER, 0, 0);
	}

	connectionSet = 1;
	networkingSetup = 1;
	//Set it to update the networking
	mData.doesUpdateNetworking = 1;

	StartBoids();
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
}

void NetworkedGameState::updateData()
{
	//Update the state and gamestate data
	if (!connectionSet)
		State::updateData();
	else if (connectionSet)
	{
		// clients in data push mode don't send packets, both data sharing peers send packets
		if ((mData.dataMethod == 1 && mData.mIsHost) || mData.dataMethod == 2)
		{
			char buffer[1024];
			int bytesWritten = 0;
			buffer[0] = ID_BOID_DATA;
			++bytesWritten;
			bytesWritten += SerializeBoids(buffer + bytesWritten);
			if (bytesWritten != 5) //If it's not just ID plus number of boids
				peer->Send(buffer, bytesWritten, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
		}
		else // Data coupled
		{

		}
	}
}

void NetworkedGameState::processBuffer()
{

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
			case ID_BOID_DATA:
				DeserializeBoids((char*)packet->data);
				break;
		}
	}
}

void NetworkedGameState::render()
{
	gpGame->beginLoop();
	gpGame->processLoop();
	exit = gpGame->endLoop();

	if (exit)
		mData.running = 0;
}

int NetworkedGameState::SerializeBoids(char* buffer)
{
	std::vector<KinematicUnit*> boids = gpGame->getLocalUnitManager()->getEnemyUnits();
	std::vector<KinematicUnit*>::iterator iter;
	int bytesWritten = 0;
	int size = boids.size();

	// KinematicUnit Data 
	// -> Kinematic
	//		-> Position (Vector2D)
	//			-> X (float)
	//			-> Y (float)
	//		-> Velocity (Vector2D)
	//			-> X (float)
	//			-> Y (float)
	//		-> Orientation (float)
	//		-> RotationVelocity (float)
	// -> KinematicUnit
	//		-> CurrentSteering (Steering)
	//			-> Linear (Vector2D)
	//				-> X (float)
	//				-> Y (float)
	//			-> Angular (float)
	//			-> ApplyDirectly (bool)
	//		-> Max Velocity (float)
	//		-> Max Acceleration (float)
	//		-> Max Angular Velocity (float)

	// Data Push
	// -> "Host" peer simulates flocking, pushes position/rotation data to "Client" peer
	// Data Sharing
	// -> Each peer has it's own flock, sends position/rotation data to each other
	// Data coupled
	// -> Peer simulates, then sends position/rotation data AS WELL AS all global steering weight variables

	memcpy(buffer, &size, sizeof(size));
	buffer += sizeof(size);
	bytesWritten += sizeof(int);

	for (iter = boids.begin(); iter != boids.end(); ++iter)
	{
		KinematicUnit* unit = *iter;

		float x = unit->getPosition().getX();
		float y = unit->getPosition().getY();
		float r = unit->getOrientation();

		memcpy(buffer, &x, sizeof(x));
		buffer += sizeof(float);
		memcpy(buffer, &y, sizeof(y));
		buffer += sizeof(float);
		memcpy(buffer, &r, sizeof(r));
		buffer += sizeof(float);

		bytesWritten += sizeof(x) + sizeof(y) + sizeof(r);
	}

	return bytesWritten;
}

void NetworkedGameState::DeserializeBoids(char* buffer)
{
	UnitManager* manager;
	std::vector<KinematicUnit*> boids;
	std::vector<KinematicUnit*>::iterator iter;
	int currentSize;
	int receivedSize;

	if (mData.dataMethod == 1)
		manager = gpGame->getLocalUnitManager();
	else
		manager = gpGame->getPeerUnitManager();

	++buffer; // jump past ID enum
	receivedSize = *(int*)buffer;
	buffer += sizeof(int); // jump past number of boids

	boids = manager->getEnemyUnits();
	currentSize = boids.size();

	if (currentSize < receivedSize)
	{
		// add units, re-get vector
		for (; currentSize < receivedSize; ++currentSize)
			manager->createUnit(Vector2D(), BOID);

		boids = manager->getEnemyUnits();
	}
	else if (currentSize > receivedSize)
	{
		// remove units, re-get vector
		for (; currentSize > receivedSize; --currentSize)
			manager->removeRandomEnemy();

		boids = manager->getEnemyUnits();
	}


	for (iter = boids.begin(); iter != boids.end(); ++iter)
	{
		KinematicUnit* unit = *iter;

		float x, y, r;

		memcpy(&x, buffer, sizeof(x));
		buffer += sizeof(float);
		memcpy(&y, buffer, sizeof(y));
		buffer += sizeof(float);
		memcpy(&r, buffer, sizeof(r));
		buffer += sizeof(float);

		unit->setPosition(x, y);
		unit->setOrientation(r);
	}
}

int NetworkedGameState::StartBoids()
{
	bool goodGame = gpGame->init();
	
	if (!goodGame)
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}
	else
	{
		gpGame->setDataMode(mData.dataMethod);
		gpGame->setIsHost(mData.mIsHost);
		
		initializedBoids = true;
		return 1;
	}
}
