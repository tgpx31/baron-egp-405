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

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "State.h"
#include "../Game.h"

class GameState : public State
{
public:
	void init(State* prev, State** currentState);

protected:
	// Data relevant to the game state
	struct GameData
	{
		// We need all the Game data here
		Game myGame;

	} mGameStateData;

	bool initializedBoids, exit;

	int StartBoids();

	State *mPrev;

	void updateData() override;
	void processBuffer() override;

	void render() override;
	void ArriveFromPreviousState(StateData *data) override;
};

#endif