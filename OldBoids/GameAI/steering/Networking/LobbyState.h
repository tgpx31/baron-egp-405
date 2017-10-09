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
#ifndef LOBBY_STATE_H
#define LOBBY_STATE_H

#include "State.h"

// Inherited from state with overrides
class LobbyState : public State
{
public:
	// Local, Host, Client
	void init(State* nextL, State* nextM, State* nextR, State** currentState);
private:
	// Data for the lobby state
	struct LobbyData
	{
		State *next1, *next2, *next3;
		int promptHost;
	} mLobbyData;

	virtual void processBuffer() override;
};

#endif