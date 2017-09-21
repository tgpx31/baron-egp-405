#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "ClientState.h"
#include <map>
#include <string>
#include <ctime>

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

	time_t timer;
};

#endif