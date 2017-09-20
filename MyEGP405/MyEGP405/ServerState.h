#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include "ClientState.h"
#include <map>
//#include <iterator>
#include <string>

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

	//int connectedClientCount;

	struct ClientDatabase
	{
		int connectedClientCount;
		std::map<int, std::string> clientDictionary;
	}mDataBase;
};

#endif