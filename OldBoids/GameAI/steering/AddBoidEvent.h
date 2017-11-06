#ifndef _ADD_BOID_EVENT_H
#define _ADD_BOID_EVENT_H

#include "Networking\Event.h"

#pragma pack(push, 1)
class AddBoidEvent : public Event
{
private:
	// data
	Vector2D center;
	Vector2D spawnPos;

public:

	AddBoidEvent(const unsigned int messageID, Vector2D d_center, Vector2D d_spawnPos);
	virtual void Execute();
};
#pragma pack (pop)

#endif // _MODIFY_WEIGHT_EVENT_H_