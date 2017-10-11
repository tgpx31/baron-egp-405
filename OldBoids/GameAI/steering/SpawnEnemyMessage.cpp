#include "SpawnEnemyMessage.h"
#include "KinematicUnit.h"
#include "Game.h"

SpawnEnemyMessage::SpawnEnemyMessage(Behavior behavior) : GameMessage(SPAWN_ENEMY_MESSAGE)
{
	mBehavior = behavior;
}

SpawnEnemyMessage::~SpawnEnemyMessage()
{
}

void SpawnEnemyMessage::process()
{
	int radius;
	switch (mBehavior)
	{
		case ARRIVE:
		case WANDER_SEEK: radius = 200; break;
		case SEEK:
		case WANDER_FLEE: radius = 100; break;
		default: break;
	}

	KinematicUnit* player = gpGame->getPlayerUnit();

	double angle = std::rand() % 361; //generate a random angle in degrees

	Vector2D spawnPos;
	spawnPos.setX(player->getPosition().getX() + std::cos(angle) * radius);
	spawnPos.setY(player->getPosition().getY() + std::sin(angle) * radius);

	gpGame->getLocalUnitManager()->createUnit(spawnPos, mBehavior);
}
