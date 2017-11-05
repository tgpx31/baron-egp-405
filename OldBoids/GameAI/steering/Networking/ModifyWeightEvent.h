#ifndef _MODIFY_WEIGHT_EVENT_H_
#define _MODIFY_WEIGHT_EVENT_H_

#include "Event.h"

#pragma pack(push, 1)
class ModifyWeightEvent : public Event
{
private:
	// data
	unsigned int steeringID;	// 1 is cohesion, 2 separation, 3 alignment
	unsigned int weight;

public:

	ModifyWeightEvent(const unsigned int messageID, const unsigned int steeringType, const unsigned int newWeight);
	virtual void Execute();
};
#pragma pack (pop)

#endif // _MODIFY_WEIGHT_EVENT_H_