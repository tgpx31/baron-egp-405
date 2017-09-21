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
#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "State.h"
#include "RakNet\BitStream.h"
#include "Messages.h"
#include <vector>

class ClientState : public State
{
public:
	// inherited
	void init(State* prev, State* nextL, State* nextR, State** currentState) override;
	void render();

protected:
	// inherited
	void updateNetworking();
	void processBuffer() override;
	void ArriveFromPreviousState(StateData *data) override;
	void updateData() override;

	// ease of use function
	void getClientInfo();

	// RakNet connection vars
	RakNet::RakPeerInterface *peer;
	RakNet::Packet *packet;

	// User specific identifiers
	char myUsername[STR_MAX];
	unsigned int id;

	//Flags
	int ipSet;
	int requestConnection;
	int infoSet;

	// Used for the message history
	std::vector<std::string> displayStrings;
};

#endif