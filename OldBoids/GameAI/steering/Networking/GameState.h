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


// State hierarchy
#include "State.h"

// Utilities
#include <sstream>
#include <assert.h>

// Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

// Game specific components
#include "../GraphicsSystem.h"
#include "../GraphicsBufferManager.h"
#include "../GraphicsBuffer.h"
#include "../SpriteManager.h"
#include "../Sprite.h"
#include "../Player.h"
//---
#include "Timer.h"
#include "../Defines.h"
#include <vector>

class GameState : public State
{
public:
	void init(State* prev, State** currentState);

protected:
	// Data relevant to the game state
	struct GameData
	{
		// Graphics
		GraphicsSystem *mpGraphicsSystem;
		GraphicsBufferManager *mpGraphicsBufferManager;
		SpriteManager *mpSpriteManager;

		// Visual sources
		IDType mBackgroundBufferID;
		IDType mPlayerIconBufferID;
		IDType mEnemyIconBufferID;
		ALLEGRO_FONT *mpFont;

		// Control
		Timer *mpLoopTimer;
		int mExit;

		ALLEGRO_KEYBOARD_STATE mPreviousKeyboardState;
		ALLEGRO_KEYBOARD_STATE mCurrentKeyboardState;

		ALLEGRO_MOUSE_STATE mPreviousMouseState;
		ALLEGRO_MOUSE_STATE mCurrentMouseState;

		// Units
		Player *mpLocalPlayer;

	} mGameState;

	// Game State functions
	int initGame();
	int cleanup();
	
	inline bool keyPressed(int keycode) { return (al_key_down(&mGameState.mCurrentKeyboardState, keycode) && !al_key_down(&mGameState.mPreviousKeyboardState, keycode)); };
	inline bool keyUp(int keycode) { return (!al_key_down(&mGameState.mCurrentKeyboardState, keycode) && al_key_down(&mGameState.mPreviousKeyboardState, keycode)); };
	//----

	int initialized, exit;

	State *mPrev;

	void updateInput() override;
	void updateData() override;
	void processBuffer() override;
	void render() override;

	void ArriveFromPreviousState(StateData *data) override;
};

#endif