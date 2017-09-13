#include <iostream>

#include "RakNet\RakPeerInterface.h"

#include "ChatClient.h"
#include "Messages.h"

void ChatClient::Init()
{
	mState = LOBBY_STATE;
	mpPeer = RakNet::RakPeerInterface::GetInstance();
	mUsername = "";

	/*
	mMaxClients = 10;
	mServerPort = 666;
	strcpy(mServerIP, "127.0.0.1");
	*/
}

void ChatClient::Run()
{
	char izard;

	std::string ip;
	unsigned int port;

	std::cout << "Welcome to the chat lobby." << std::endl << std::endl;

	std::cout << "What is your name?" << std::endl;
	std::cin >> mUsername;
	std::cout << "Hello, " << mUsername << std::endl << std::endl;

	std::cout << "(C)lient or (S)erver?" << std::endl;
	std::cin >> izard;

	// TODO: Add input validation
	if (izard == 'c' || izard == 'C')
	{
		mState = CLIENT_STATE;

		std::cout << "Input server IP, or press enter for 127.0.0.1" << std::endl;
		std::cin >> ip;
		if (ip == "")
			ip = "127.0.0.1";
		std::cout << std::endl;
	}
	else
	{
		mState = SERVER_STATE;
	}

	std::cout << "Input server port, or press enter for 11111" << std::endl;
	std::cin >> port;
	if (port == NULL)
		port = 11111;
	std::cout << std::endl;

	if (mState == CLIENT_STATE)
		RunClientLoop(ip, port);
	else
		RunServerLoop(port);
}

void ChatClient::RunClientLoop(std::string ip, unsigned int serverPort)
{
	RakNet::SocketDescriptor sd;
	mpPeer->Startup(1, &sd, 1);
	mpPeer->Connect(ip.c_str(), serverPort, 0, 0);

	while (1)
	{
		// CLIENT LOGIC GOES HERE
	}
}

void ChatClient::RunServerLoop(unsigned int serverPort)
{
	RakNet::SocketDescriptor sd(serverPort, 0);
	mpPeer->Startup(10, &sd, 1);
	mpPeer->SetMaximumIncomingConnections(10);

	while (1)
	{
		// SERVER LOGIC GOES HERE
	}
}