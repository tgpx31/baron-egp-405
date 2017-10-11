#include "SpawnBoidMessage.h"
#include "Game.h"

SpawnBoidMessage::SpawnBoidMessage() : GameMessage(SPAWN_BOID_MESSAGE)
{
}

SpawnBoidMessage::~SpawnBoidMessage()
{
}

void SpawnBoidMessage::process()
{
	Vector2D center(512, 384);
	Vector2D spawnPos;

	double angle = std::rand() % 361;
	double radius = std::rand() % 100;

	spawnPos.setX(center.getX() + std::cos(angle) * radius);
	spawnPos.setY(center.getY() + std::sin(angle) * radius);

	gpGame->getLocalUnitManager()->createUnit(spawnPos, BOID);
}
