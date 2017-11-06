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

#ifndef NETWORKED_GAME_STATE_H
#define NETWORKED_GAME_STATE_H

#include "GameState.h"
#include "DeanQueue.h"

#define DEFAULT_PORT_NUMBER 666
#define MAX_PEER_CONNECTIONS 2

class NetworkedGameState : public GameState
{
public:
	void init(State* prev, State** currentState);

private:
	int connectionSet, networkingSetup, isPlaying, isX, willSendState;

	std::vector<Player*> otherPlayers;	// list of all connected players
	Sprite *remotePlayerSprite;

	RakNet::RakPeerInterface *peer;
	RakNet::Packet *packet;
	char connectionAddress[STR_MAX];

	RakNet::SystemAddress address;
	DeanQueue eventQueue;

	inline int Write(char* buffer, void* data) { const char *const start = buffer; memcpy(buffer, data, sizeof(data)); buffer += sizeof(data); return (buffer - start); };
	int SerializePlayer(char* buffer);
	void DeserializePlayers(char *buffer);

	void updateInput() override;
	void updateData() override;
	void updateNetworking() override;
	
	void render() override;

	void ArriveFromPreviousState(StateData *data) override;

};
#endif