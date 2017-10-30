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

#ifndef STATE_H
#define STATE_H

#include <stdio.h>
#include "RakNet\RakPeerInterface.h"


#define STR_MAX 256

class State abstract
{
	public: 
		State();
		virtual void init(State** currentState);

		virtual void update();
		virtual void render();
		inline int isRunning() { return mData.running; };

	protected:
		// All data that every state will have in common
		struct StateData
		{
			int running,
				mIsHost,
				doesUpdateInput, 
				doesUpdateNetworking, 
				doesUpdateState, 
				// flip this flag whenever you want to display your buffer and prompt
				doesDisplay,
				dataMethod; // 1 = data push, 2 = data sharing, 3 = data coupled

			unsigned char keyboard[STR_MAX]; //Keyboard state array
			unsigned int bufferIndex; //Index to end of buffer
			char buffer[STR_MAX],
				promptBuffer[STR_MAX];

		} mData;

		State **mCurrentState;

		// Update loop component functions
		virtual void updateInput();
		virtual void updateData();
		virtual void updateNetworking() {};
		virtual void processBuffer();

		// State jumpers
		void GoToNextState(State* nextState);
		virtual void ArriveFromPreviousState(StateData *data);

		// convenience function
		void clearBuffer();
};

#endif