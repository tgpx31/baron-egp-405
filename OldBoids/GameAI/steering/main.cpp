/*main.cpp
 *	Adapted from: Example program for the Allegro library, by Shawn Hargreaves.
 *	Sets up all systems and runs the main game loop until it is time to quit.
 *
 *	Dean Lawson
 *	Champlain College
 *	2011
 *
 */
#define ALLEGRO_USE_CONSOLE

#include "Networking\LobbyState.h"
#include "Networking\NetworkedGameState.h"

int main(int argc, char **argv)
{
	#pragma region OldBoidsMain
	//gpPerformanceTracker = new PerformanceTracker();
	//gpPerformanceTracker->startTracking("init");

	////create the global game object
	gpGame = new Game;
	////init the game
	//bool goodGame = gpGame->init();
	//if(!goodGame) 
	//{
	//	fprintf(stderr, "failed to initialize Game object!\n");
	//	return -1;
	//}

	//gpPerformanceTracker->stopTracking("init");
	//cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";

	//bool shouldExit = false;

	////game loop
	//while( !shouldExit )
	//{
	//	gpPerformanceTracker->clearTracker("loop");
	//	gpPerformanceTracker->startTracking("loop");

	//	gpGame->beginLoop();

	//	gpPerformanceTracker->clearTracker("draw");
	//	gpPerformanceTracker->startTracking("draw");

	//	gpGame->processLoop();

	//	gpPerformanceTracker->stopTracking("draw");

	//	shouldExit = gpGame->endLoop();

	//	gpPerformanceTracker->stopTracking("loop");
	//	cout << "loop took:" << gpPerformanceTracker->getElapsedTime("loop") << "ms";
	//	cout << "draw took:" << gpPerformanceTracker->getElapsedTime("draw") << "ms\n";

	//}

	//gpGame->cleanup();
	//delete gpGame;
	//gpGame = NULL;

	//delete gpPerformanceTracker;
	//gMemoryTracker.reportAllocations( cout );
#pragma endregion

	// We should still have a lobby state structure like in our previous project/lab
	State *CurrentState;	// Starts in the Lobby State by default
	LobbyState lobby[1];
	GameState localGame[1];
	//NetworkedGameState networkedGame[1];	// Can get away with using only one instance of this

	lobby->init(localGame, nullptr, nullptr, &CurrentState);
	//localGame->init(lobby, &CurrentState);
	//networkedGame->init(lobby, &CurrentState);

	CurrentState = lobby;

	while (CurrentState->isRunning())
	{
		CurrentState->update();
	}
	system("pause");
	return 0;
}