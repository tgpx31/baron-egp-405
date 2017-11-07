#ifndef _EVENT_H_
#define _EVENT_H_

#include "Messages.h"
#include <stdio.h>
#include <Windows.h>

#include "../Player.h"

class NetworkedGameState;

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

class ExitGameEvent : public Event
{
private:
	int mID;
	NetworkedGameState* mGame;

public:
	ExitGameEvent(int id, NetworkedGameState* game) { mID = id; mGame = game; };

	void Execute() override;
	inline int getID() { return mID; };
};

class FlipSpriteEvent : public Event
{
private:
	int mID;
	Player *player;
	Sprite *sprite;

public:
	FlipSpriteEvent(int id, Player *play, Sprite *spr) { mID = id; player = play; sprite = spr; };

	void Execute() override;
	inline int getID() { return mID; };
};

class ChangeScaleEvent : public Event
{
private:
	int mID;
	Player *player;

	float mScale;

public:
	ChangeScaleEvent(int id, float scale, Player *play) { mID = id; mScale = scale; player = play; };

	void Execute() override;
	inline int getID() { return mID; };
};

class ChangeOrientationEvent : public Event
{
private:
	int mID;
	Player *player;

	float mOrientation;

public:
	ChangeOrientationEvent(int id, float orientation, Player *ply) { mID = id; mOrientation = orientation; player = ply; };

	void Execute() override;
	inline int getID() { return mID; };
};
#pragma pack (pop)


#endif // _EVENT_H_