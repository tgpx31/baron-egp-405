#pragma once

#include <allegro5\mouse.h>
#include <allegro5\keyboard.h>
#include <allegro5\keycodes.h>
#include <allegro5\allegro_font.h>
#include "Trackable.h"

class InputManager : public Trackable
{
private:
	ALLEGRO_KEYBOARD_STATE mPreviousKeyboardState;
	ALLEGRO_KEYBOARD_STATE mCurrentKeyboardState;
	
	ALLEGRO_MOUSE_STATE mPreviousMouseState;
	ALLEGRO_MOUSE_STATE mCurrentMouseState;
	
	ALLEGRO_FONT* mpFont;
	
	bool keyPressed(int keycode);

public:
	InputManager();
	~InputManager();

	void setFont(ALLEGRO_FONT* pFont) { mpFont = pFont; }

	bool init();
	void update();
	void draw();
};