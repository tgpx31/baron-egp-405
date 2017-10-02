#include "DeleteEnemyMessage.h"
#include "Game.h"

DeleteEnemyMessage::DeleteEnemyMessage() : GameMessage(DELETE_ENEMY_MESSAGE)
{
}

DeleteEnemyMessage::~DeleteEnemyMessage()
{
}

void DeleteEnemyMessage::process()
{
	gpGame->getUnitManager()->removeRandomEnemy();
}
