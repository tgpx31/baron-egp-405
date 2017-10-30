#ifndef _EXIT_GAME_EVENT_H_
#define _EXIT_GAME_EVENT_H_

#include "Event.h"

class ExitGameEvent : public Event
{
private:
	// data
	// none needed for this
public:

	ExitGameEvent() {};
	virtual void Execute() { gpGame->endGame(); };
};

#endif // _EXIT_GAME_EVENT_H_