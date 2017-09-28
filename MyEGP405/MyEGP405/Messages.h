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
#ifndef MESSAGES_H
#define MESSAGES_H

#include "RakNet\MessageIdentifiers.h"
#include "RakNet\RakNetTypes.h"

// Message structs and identifiers
// all pretty self explanatory

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,

	// New messages
	ID_VALID_MOVE,
	ID_
};

/*
#pragma pack (push, 1)
struct UsernameMessage		// this structure can be used for multiple message ids
{
	char messageID;
	char username[31];
};

struct ClientNumberMessage
{
	char messageID;
	unsigned int clientNumber;
};

struct ClientChatMessage
{
	char messageID;
	int uniqueID;
	char destination[31];
	char message[512];
};

struct ServerChatMessage
{
	char messageID;
	int isWhisper;
	char message[512];
	char username[31];
};
#pragma pack (pop)
*/

#pragma pack (push, 1)
//Structure for movement messages
struct GameMoveMessage
{
	int id;
	int placementIndex;
};
#pragma pack (pop)

#endif
