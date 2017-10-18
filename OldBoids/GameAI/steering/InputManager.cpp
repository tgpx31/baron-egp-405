#include "InputManager.h"

#include <allegro5\allegro_ttf.h>
#include <sstream>
#include "Game.h"
#include "GraphicsSystem.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
	//Clean up allegro input systems
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

bool InputManager::init()
{
	//Initialize allegro input systems

	if (!al_install_keyboard())
	{
		printf("Keyboard not installed!\n");
		return false;
	}

	if (!al_install_mouse())
	{
		printf("Mouse not installed!\n");
		return false;
	}

	//hide the mouse
	if (!al_hide_mouse_cursor(gpGame->getGraphicsSystem()->getDisplay()))
	{
		printf("Mouse cursor not able to be hidden!\n");
		return false;
	}

	return true;
}

void InputManager::update()
{
	//Get Input
	al_get_mouse_state(&mCurrentMouseState);
	al_get_keyboard_state(&mCurrentKeyboardState);
	
	/*
	//left mouse click
	if (al_mouse_button_down(&mCurrentMouseState, 1) && !al_mouse_button_down(&mPreviousMouseState, 1))
	{
		Vector2D pos(mCurrentMouseState.x, mCurrentMouseState.y);
		GameMessage* pMessage = new PlayerMoveToMessage(pos);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	*/

	//if escape key was down then exit the loop
	if (keyPressed(ALLEGRO_KEY_ESCAPE))
	{
		/*GameMessage* pMessage = new EndGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);*/
	}

	mPreviousKeyboardState = mCurrentKeyboardState;
	mPreviousMouseState = mCurrentMouseState;
}

void InputManager::draw()
{
	//create mouse text
	std::stringstream mousePos;
	mousePos << mCurrentMouseState.x << ":" << mCurrentMouseState.y;
	
	//write text at mouse position
	al_draw_text(mpFont, al_map_rgb(255, 255, 255), mCurrentMouseState.x, mCurrentMouseState.y, ALLEGRO_ALIGN_CENTRE, mousePos.str().c_str());
}

bool InputManager::keyPressed(int keycode)
{
	//Return true if key was up in previous keyboard state, and down in current keyboard state
	return (al_key_down(&mCurrentKeyboardState, keycode) && !al_key_down(&mPreviousKeyboardState, keycode));
}
