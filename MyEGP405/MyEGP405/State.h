#ifndef STATE_H
#define STATE_H

#include "RakNet\RakPeerInterface.h"

enum ServerStates
{
	LOBBY_STATE,
	CLIENT_STATE,
	SERVER_STATE
};

// All data that every state will have in common
struct StateData
{
	int running; //Whether or not the application is running

	ServerStates state; //Application's current state

	char connectionAddress[512]; // IP of connection to connect to
	unsigned int port; //Port the server will be connecting to
	char username[31]; //Username of the user

	unsigned char keyboard[256]; //Keyboard state array
	char buffer[256]; //Buffer to store input
	unsigned int bufferIndex; //Index to end of buffer
};

class State
{
	public: 
		StateData mData;

		State();

		void update();
		void render();

	protected:
		void updateInput();
		void updateData();
		void processBuffer();
};

#endif