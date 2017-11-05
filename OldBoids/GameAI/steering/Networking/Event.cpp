#include "Event.h"

void MovePlayerEvent::Execute()
{
	player->setVelocity(mVel);
}
