#ifndef MESSAGES_H
#define MESSAGES_H

#include "RakNet\MessageIdentifiers.h"
#include "RakNet\RakNetTypes.h"

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,

	// Handshake exchange
	ID_USERNAME,			// Client responds to connection by sending its username to server, server assigns username to your address
	ID_NEW_CLIENT_JOIN,		// server broadcasts welcome message to all clients
	ID_CLIENT_NUMBER,		// server associates username with client number

	// Messaging exchange
	ID_CHAT_MESSAGE,		// send request by anyone

	// Misc.
	ID_SEND_ALL,			// sent by client, all current usernames!
};

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

struct ChatMessage
{
	char messageID;
	char destination[31];
	char message[512];
};
#pragma pack (pop)

#endif
