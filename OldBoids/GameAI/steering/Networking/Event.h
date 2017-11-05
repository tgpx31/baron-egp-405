#ifndef _EVENT_H_
#define _EVENT_H_

#include "Messages.h"
#include <stdio.h>
#include <Windows.h>

#include "../Player.h"

#pragma pack (push, 1)
class Event
{
protected:
	int id = 0;

public:
	virtual void Execute() {};
};

class MovePlayerEvent : public Event
{
private:
	Vector2D mVel;
	Player *player;
	int mID;

public:
	MovePlayerEvent(int idType, Vector2D vel, Player * ply, int id) { id = idType; mVel = vel; player = ply; mID = id; };

	void Execute() override;
	inline int getID() { return mID; };
	inline void setPlayer(Player* ply) { player = ply; };
};
#pragma pack (pop)


#endif // _EVENT_H_