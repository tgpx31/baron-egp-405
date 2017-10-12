/*main.cpp
 *	Adapted from: Example program for the Allegro library, by Shawn Hargreaves.
 *	Sets up all systems and runs the main game loop until it is time to quit.
 *
 *	Dean Lawson
 *	Champlain College
 *	2011
 *
 */

// This file wasmodified by Brian Baron, Colin Brady, and Justin Mulkin with permission from author.

#define ALLEGRO_USE_CONSOLE

#include "Networking\LobbyState.h"
#include "Networking\NetworkedGameState.h"

#include "../DeanLib/MemoryTracker.h"
#include "../DeanLib/PerformanceTracker.h"

PerformanceTracker* gpPerformanceTracker = NULL;

int main(int argc, char **argv)
{
	gpPerformanceTracker = new PerformanceTracker();
	gpGame = new Game();

	// We should still have a lobby state structure like in our previous project/lab
	State *CurrentState;	// Starts in the Lobby State by default
	LobbyState lobby[1];
	GameState localGame[1];
	NetworkedGameState networkedGame[1];	// Can get away with using only one instance of this

	lobby->init(localGame, networkedGame, nullptr, &CurrentState);
	localGame->init(lobby, &CurrentState);
	networkedGame->init(lobby, &CurrentState);

	CurrentState = lobby;

	while (CurrentState->isRunning())
	{
		CurrentState->update();
	}
	system("pause");

	gpGame->cleanup();
	delete gpGame;
	gpGame = NULL;

	delete gpPerformanceTracker;
	gMemoryTracker.reportAllocations( std::cout );
	return 0;
}