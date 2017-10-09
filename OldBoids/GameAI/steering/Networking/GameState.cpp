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
#include "GameState.h"


void GameState::init(State * prev = nullptr, State ** currentState = nullptr)
{
	if (currentState != nullptr)
		State::init(currentState);

	mData.doesUpdateInput = 1;
	mData.doesUpdateState = 1;
	mData.doesUpdateNetworking = 0;
	mData.doesDisplay = 0;

	if (prev != nullptr)
		mPrev = prev;

	strcpy(mData.promptBuffer, "BOIDS\n");

	initializedBoids = false;
	exit = false;
}

void GameState::updateData()
{
	
}

void GameState::processBuffer()
{
	clearBuffer();
}

void GameState::render()
{
	/*gpPerformanceTracker->clearTracker("loop");
	gpPerformanceTracker->startTracking("loop");*/

	gpGame->beginLoop();
	/*gpPerformanceTracker->clearTracker("draw");
	gpPerformanceTracker->startTracking("draw");*/

	gpGame->processLoop();
	//gpPerformanceTracker->stopTracking("draw");

	exit = gpGame->endLoop();

	/*gpPerformanceTracker->stopTracking("loop");
	std::cout << "loop took:" << gpPerformanceTracker->getElapsedTime("loop") << "ms";
	std::cout << "draw took:" << gpPerformanceTracker->getElapsedTime("draw") << "ms\n";*/

	if (exit)
		mData.running = 0;
}

void GameState::ArriveFromPreviousState(StateData *data)
{
	init();
	mData.doesDisplay = 1;
	StartBoids();
}

int GameState::StartBoids()
{
	//gpGame = new Game();
	gpPerformanceTracker->startTracking("init");

	bool goodGame = gpGame->init();
	if (!goodGame)
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}
	else
	{
		gpPerformanceTracker->stopTracking("init");
		std::cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";

		initializedBoids = true;
		return 1;
	}
}