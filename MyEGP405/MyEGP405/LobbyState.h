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
#ifndef LOBBY_STATE_H
#define LOBBY_STATE_H

#include "State.h"

// Inherited from state with overrides
class LobbyState : public State
{
public:
	virtual void init(State* prev, State* nextL, State* nextR, State** currentState) override;
private:
	virtual void ArriveFromPreviousState(StateData *data) override;
	virtual void processBuffer() override;
	virtual void updateNetworking() {};
};

#endif