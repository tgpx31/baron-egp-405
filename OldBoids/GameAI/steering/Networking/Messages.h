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
	ID_PLAYERS_DATA,
	ID_BOID_DATA_AND_GLOBALS,
	ID_MODIFY_WEIGHTS,
	ID_MOVE_PLAYER
};

#endif
