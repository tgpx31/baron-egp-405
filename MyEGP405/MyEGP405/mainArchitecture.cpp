#include <Windows.h>
#include "RakNet\RakPeerInterface.h"
// INTRO TO NETWORKED ARCHITECTURE - ASYNC CHAT ROOM SETUP

struct KeyboardState
{
	unsigned char keyboard[256];
};

struct ApplicationState
{
	int running;

	KeyboardState key[1];
};

void updateInput(KeyboardState *key)
{
	/*	get mouse, keyboard, controller, etc state
		track this for later
	*/

	// example: in a chatroom...
	// get all keys at the same time
	GetKeyboardState(key->keyboard);
}

void updateNetworking(/*param*/)
{
	// TMP: make some networking state
	//		that stores a peer instance
	// for now a temp
	RakNet::RakPeerInterface *peer;


	/*	recieve packets until no more packets
	 *	while packet exsists:
	 *		process message
	 *		track data (if any) for later
	*/

	RakNet::Packet *packet;
	for (packet = peer->Receive();
		packet;
		peer->DeallocatePacket(packet),
		peer->Receive())
	{
		// HOORAY NETWORKING MESSAGES
	}
	// strictly processing packets
}

void updateState(ApplicationState *state)
{
	/*	account for all previous updates here
	 *	change application with respect to time and other updates
	*/

	// e.g. chat room
	//	- update chat stream
}

void render(const ApplicationState *state)
{
	/*	"clear buffer"
	 * system("cls");
	 *	display state to display medium
	*/
	// e.g. chat room
	//	display chat stream (dump the whole queue to the console) this is still rendering
}

int main(void)
{
	ApplicationState theState[1];
	theState->running = 1;

	while (theState->running)	// Game Loop, NOT the networking loop
	{
		/* Get input
		 * use input
		 * draw the result
		 */
		updateInput(theState->key);
		updateNetworking(/*param*/);
		updateState(theState);
		render(theState);
	}

	return 0;
}