#ifndef STATE_H
#define STATE_H

#include "RakNet\RakPeerInterface.h"

enum ServerStates
{
	LOBBY_STATE,
	CLIENT_STATE,
	SERVER_STATE
};



class State abstract
{
	public: 
		State();

		virtual void update() = 0;
		virtual void render() = 0;
		void init(State* prev, State* nextL, State* nextR, State** currentState);
		inline int isRunning() { return mData.running; };

		State* operator=(State* other);

	protected:
		// All data that every state will have in common
		struct StateData
		{
			int running; //Whether or not the application is running

			char connectionAddress[512]; // IP of connection to connect to
			unsigned int port; //Port the server will be connecting to
			char username[31]; //Username of the user

			unsigned char keyboard[256]; //Keyboard state array
			char buffer[256]; //Buffer to store input
			unsigned int bufferIndex; //Index to end of buffer
		} mData;

		State *mPrev, *mNextL, *mNextR;
		State **mCurrentState;

		void updateInput();
		virtual void updateData() = 0;
		virtual void updateNetworking() = 0;
		virtual void processBuffer() = 0;

		void GoToNextState(State* nextState);
		virtual void ArriveFromPreviousState(StateData *data);
};

#endif