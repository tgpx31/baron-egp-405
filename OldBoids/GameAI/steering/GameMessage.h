#pragma once

#include "Trackable.h"

enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE,
	SPAWN_ENEMY_MESSAGE,
	DELETE_ENEMY_MESSAGE,
	END_GAME_MESSAGE,
	TOGGLE_DEBUG_MESSAGE,
	CHANGE_UI_MODE_MESSAGE,
	EDIT_DEBUG_VALUE_MESSAGE,
	SPAWN_BOID_MESSAGE
};

class GameMessage: public Trackable
{
public:
	friend class GameMessageManager;

	GameMessage( MessageType type  );
	~GameMessage();

	double getSubmittedTime() const { return mSubmittedTime; };
	double getScheduledTime() const { return mScheduledTime; };

protected:
	MessageType mType;
	double mSubmittedTime;
	double mScheduledTime;

private:
	virtual void process() = 0; 
};

