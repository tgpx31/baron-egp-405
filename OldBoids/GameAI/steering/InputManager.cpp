#include "InputManager.h"
#include "GameMessage.h"
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"
#include "EndGameMessage.h"
#include "SpawnEnemyMessage.h"
#include "DeleteEnemyMessage.h"
#include "ToggleDebugMessage.h"
#include "ChangeDebugModeMessage.h"
#include "EditDebugValueMessage.h"
#include "SpawnBoidMessage.h"
#include <allegro5\allegro_ttf.h>
#include <sstream>
#include "Game.h"
#include "GraphicsSystem.h"
#include "Behaviors.h"

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
	
	// if local, use the message system from old boids
	// if networked, use the event queue and management system
	if (gpGame->getDataMode() == 0)
	{
		//if escape key was down then exit the loop
		if (keyPressed(ALLEGRO_KEY_ESCAPE))
		{
			GameMessage* pMessage = new EndGameMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}

		if (keyPressed(ALLEGRO_KEY_I))
		{
			GameMessage* pMessage0 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage0, 0);
			GameMessage* pMessage1 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage1, 0);
			GameMessage* pMessage2 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage2, 0);
			GameMessage* pMessage3 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage3, 0);
			GameMessage* pMessage4 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage4, 0);
		}
		if (keyPressed(ALLEGRO_KEY_D))
		{
			GameMessage* pMessage = new DeleteEnemyMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}

		//Debug Mode inputs
		if (keyPressed(ALLEGRO_KEY_TILDE))
		{
			GameMessage* pMesssage = new ToggleDebugMessage();
			MESSAGE_MANAGER->addMessage(pMesssage, 0);
		}

		//Only do this if in debug state
		if (gpGame->getDebug())
		{
			if (keyPressed(ALLEGRO_KEY_V))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(VELOCITY);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_L))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(ACCELERATION);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_U))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(ANGULAR);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_C))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(COHESION);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_S))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(SEPARATION);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_A))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(ALIGNMENT);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_M))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(VELOCITY_MATCHING);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_EQUALS))
			{
				GameMessage* pMessage = new EditDebugValueMessage(5.0f);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_MINUS))
			{
				GameMessage* pMessage = new EditDebugValueMessage(-5.0f);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
		}
	}
	else
	{
		//if escape key was down then exit the loop
		if (keyPressed(ALLEGRO_KEY_ESCAPE))
		{
			GameMessage* pMessage = new EndGameMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}

		if (keyPressed(ALLEGRO_KEY_I))
		{
			GameMessage* pMessage0 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage0, 0);
			GameMessage* pMessage1 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage1, 0);
			GameMessage* pMessage2 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage2, 0);
			GameMessage* pMessage3 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage3, 0);
			GameMessage* pMessage4 = new SpawnBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage4, 0);
		}
		if (keyPressed(ALLEGRO_KEY_D))
		{
			GameMessage* pMessage = new DeleteEnemyMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}

		//Debug Mode inputs
		if (keyPressed(ALLEGRO_KEY_TILDE))
		{
			GameMessage* pMesssage = new ToggleDebugMessage();
			MESSAGE_MANAGER->addMessage(pMesssage, 0);
		}

		//Only do this if in debug state
		if (gpGame->getDebug())
		{
			if (keyPressed(ALLEGRO_KEY_V))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(VELOCITY);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_L))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(ACCELERATION);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_U))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(ANGULAR);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_C))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(COHESION);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_S))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(SEPARATION);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_A))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(ALIGNMENT);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_M))
			{
				GameMessage* pMesssage = new ChangeDebugModeMessage(VELOCITY_MATCHING);
				MESSAGE_MANAGER->addMessage(pMesssage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_EQUALS))
			{
				GameMessage* pMessage = new EditDebugValueMessage(5.0f);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
			if (keyPressed(ALLEGRO_KEY_MINUS))
			{
				GameMessage* pMessage = new EditDebugValueMessage(-5.0f);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
			}
		}
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
