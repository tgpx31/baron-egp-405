/*
	EGP405 Networking for Online Games
	EGP Networking Playground
	By Dan Buckstein (c) 2017

	Server main file.

	Additional contributions: 
*/

//-----------------------------------------------------------------------------


#include "egpNet/server/egpServerApplicationState.h"


#include <stdio.h>
#include <Windows.h>


//-----------------------------------------------------------------------------
// MAIN
//-----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	printf("\n----------------------------------------");
	printf("\n--     EGP NET GAME ****SERVER****    --");
	printf("\n----------------------------------------");
	printf("\n\n");


	// create application interface
	egpServerApplicationState appState_server[1];

	const unsigned int terminateKey = 27;	// esc

	// start networking
	appState_server->InitializePeer(4, 0, appState_server->GetDefaultPort());

	// console persists until told to stop
	int running = 1;
	while (running)
	{
		if (!GetAsyncKeyState(terminateKey))
		{
			appState_server->OnIdle();
		}
		else
		{
			running = 0;
		}
	}

	// stop networking
	appState_server->TerminatePeer();
}



//-----------------------------------------------------------------------------
