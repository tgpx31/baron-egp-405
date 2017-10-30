#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdio.h>
#include <Windows.h>
#include "../Game.h"

class Event
{
protected:
	int id = 0;

public:
	virtual void Execute() {};
};

#endif // _EVENT_H_