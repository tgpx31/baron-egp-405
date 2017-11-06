#include "AddBoidEvent.h"

// ModifyWeightEvent ------------------------------------
AddBoidEvent::AddBoidEvent(const unsigned int messageID, Vector2D d_center, Vector2D d_spawnPos)
{
	id = messageID;
	center = d_center;
	spawnPos = d_spawnPos;
}

void AddBoidEvent::Execute()
{
	double angle = std::rand() % 361;
	double radius = std::rand() % 100;

	spawnPos.setX(center.getX() + std::cos(angle) * radius);
	spawnPos.setY(center.getY() + std::sin(angle) * radius);

	gpGame->getLocalUnitManager()->createUnit(spawnPos, BOID);
}