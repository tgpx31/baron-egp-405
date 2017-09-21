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
#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "ClientState.h"
#include <map>
#include <string>
#include <ctime>
#include <fstream>

class ServerState : public ClientState
{
public:
	void init(State* prev, State* nextL, State* nextR, State** currentState) override;
private:
	void updateNetworking();
	void processBuffer() override;
	void ArriveFromPreviousState(StateData *data) override;

	int maxClients;
	int isServer;

	struct ClientInfo
	{
		char username[31];
		RakNet::SystemAddress address;
	};

	struct ClientDatabase
	{
		int connectedClientCount;
		std::map<int, ClientInfo> clientDictionary;
	}mDataBase;
};

#endif