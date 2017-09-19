#ifndef STATE_H
#define STATE_H

#include "RakNet\RakPeerInterface.h"
#include <stdio.h>

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

		virtual void update();
		virtual void render();
		virtual void init(State* prev, State* nextL, State* nextR);
		inline int isRunning() { return mData.running; };

		State* operator=(State*other);

	protected:
		// All data that every state will have in common
		struct StateData
		{
			int running; //Whether or not the application is running
			int doesUpdateInput, doesUpdateNetworking, doesUpdateState, doesDisplay;


			char connectionAddress[512]; // IP of connection to connect to
			unsigned int port; //Port the server will be connecting to
			char username[31]; //Username of the user

			unsigned char keyboard[256]; //Keyboard state array
			char buffer[256]; //Buffer to store input
			char promptBuffer[256];

			unsigned int bufferIndex; //Index to end of buffer
		} mData;

		State *mPrev, *mNextL, *mNextR;
		State **mCurrentState;

		void updateInput();
		virtual void updateData();
		virtual void updateNetworking() = 0;
		virtual void processBuffer() = 0;

		void GoToNextState(State* nextState);
		virtual void ArriveFromPreviousState(StateData *data);
};

#endif