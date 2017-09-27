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
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "State.h"

#define BOARD_SLOTS 9

class GameState : public State
{
public:
	void init(State* prev, State** currentState);

	void render() override;

protected:
	// Data relevant to the game state
	struct GameData
	{
		State *mPrev;

		char board[STR_MAX];
		char tmpBoard[STR_MAX];
		int boardSpaceOffsets[BOARD_SLOTS];

		// The game needs to know
		// Whose turn, X or O?
		// What space is currently selected?
		// Did they win with that move?
		int initialPlayerPriority;
		int playerPriority;	// 0 is o, 1 is x
		int selectedSpace;
		char currentPlayerChar;

		int winner;			// Default this to -1. 0 = o win, 1 = x win

	} mGameStateData;

	void updateData() override;
	void processBuffer() override;


	void ArriveFromPreviousState(StateData *data) override;

	// GameState specific
	void initializeBoard();
	int validateMove();
	int checkWin();
};

#endif