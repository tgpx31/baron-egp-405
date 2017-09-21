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
#ifndef STATE_H
#define STATE_H

#include "RakNet\RakPeerInterface.h"
#include <stdio.h>

#define STR_MAX 256

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
		virtual void init(State* prev, State* nextL, State* nextR, State** currentState);
		inline int isRunning() { return mData.running; };

	protected:
		// All data that every state will have in common
		struct StateData
		{
			int running; //Whether or not the application is running
			int doesUpdateInput, doesUpdateNetworking, doesUpdateState, doesDisplay;


			char connectionAddress[512]; // IP of connection to connect to
			unsigned int port; //Port the server will be connecting to

			unsigned char keyboard[STR_MAX]; //Keyboard state array
			char buffer[STR_MAX]; //Buffer to store input
			char promptBuffer[STR_MAX];

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

		void clearBuffer();
};

#endif