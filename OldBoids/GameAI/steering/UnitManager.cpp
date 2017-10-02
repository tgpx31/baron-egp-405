#include "UnitManager.h"
#include "EndGameMessage.h"
#include "GameMessage.h"
#include "GameMessageManager.h"
#include "Game.h"

UnitManager::UnitManager(Sprite* pEnemySprite)
{
	mpEnemySprite = pEnemySprite;
}

UnitManager::~UnitManager()
{
	clear();
}

void UnitManager::addUnit(KinematicUnit* pUnit, const UnitKey& key)
{
	std::map<UnitKey, KinematicUnit*>::iterator iter = mMap.find(key);

	if (iter == mMap.end())
	{
		mMap[key] = pUnit;
	}
}

/*
UnitKey UnitManager::createUnit(const UnitKey& key, Sprite * pSprite, const Vector2D & position, float orientation, const Vector2D & velocity, float rotationVel, float maxVelocity, float maxAcceleration)
{
	KinematicUnit* newUnit = new KinematicUnit(pSprite, position, orientation, velocity, rotationVel, maxVelocity, maxAcceleration);
	addUnit(newUnit, key);

	return key;
}
*/

void UnitManager::createUnit(const Vector2D & position, Behavior behavior)
{
	KinematicUnit* newUnit = new KinematicUnit(mpEnemySprite, position, 0.0f, Vector2D(), 0.0f, gpGame->getMaxVelocity(), gpGame->getMaxAcceleration(), gpGame->getMaxAngularVelocity());
	
	switch (behavior)
	{
		case SEEK: newUnit->dynamicSeek(getUnit(PLAYER_ID)); break;
		case ARRIVE: newUnit->dynamicArrive(getUnit(PLAYER_ID)); break;
		case WANDER_SEEK: newUnit->dynamicWanderAndSeek(getUnit(PLAYER_ID), false); break;
		case WANDER_FLEE: newUnit->dynamicWanderAndSeek(getUnit(PLAYER_ID), true); break;
		case BOID: newUnit->boid(); break;
		default: break;
	}

	addUnit(newUnit, ENEMY_ID + std::to_string(mMap.size()));
}

void UnitManager::removeUnit(KinematicUnit* pUnit)
{
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		if (iter->second == pUnit)
		{
			delete iter->second;
			mMap.erase(iter);
		}
	}
}

void UnitManager::removeUnit(const UnitKey& key)
{
	std::map<UnitKey, KinematicUnit*>::iterator iter = mMap.find(key);

	if (iter != mMap.end())
	{
		delete iter->second;
		mMap.erase(iter);
	}
}

void UnitManager::removeRandomEnemy()
{
	if (mMap.size() <= 1)
		return;

	int rnd = rand() % mMap.size();

	std::map<UnitKey, KinematicUnit*>::iterator iter = mMap.begin();

	do
	{
		for (int i = 0; i < rnd; i++)
		{
			iter++;
		}
	} while (iter->first == PLAYER_ID);

	delete iter->second;
	mMap.erase(iter);

	if (mMap.size() == 0)
	{
		GameMessage* pMessage = new EndGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
}

void UnitManager::updateMaxVelocity()
{
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	float vel = gpGame->getMaxVelocity();

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		iter->second->setMaxVelocity(vel);
	}
}

void UnitManager::updateMaxAcceleration()
{
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	float accel = gpGame->getMaxAcceleration();

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		iter->second->setMaxAcceleration(accel);
	}
}

void UnitManager::updateMaxAngularVelocity()
{
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	float aVel = gpGame->getMaxAngularVelocity();

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		iter->second->setMaxAngularVelocity(aVel);
	}
}

KinematicUnit* UnitManager::getUnit(const UnitKey& key)
{
	std::map<UnitKey, KinematicUnit*>::const_iterator iter = mMap.find(key);

	if (iter != mMap.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

std::vector<KinematicUnit*> UnitManager::getEnemyUnits()
{
	std::vector<KinematicUnit*> targets;
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		if (iter->first != "player")
		{
			targets.push_back(iter->second);
		}
	}

	return targets;
}

void UnitManager::update(float time)
{
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		iter->second->update(time);
	}
}

void UnitManager::draw(GraphicsBuffer* pBuffer, bool drawCollider)
{
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		iter->second->draw(pBuffer, drawCollider);
	}
}

void UnitManager::clear()
{
	std::map<UnitKey, KinematicUnit*>::iterator iter;

	for (iter = mMap.begin(); iter != mMap.end(); iter++)
	{
		KinematicUnit* pUnit = iter->second;
		delete pUnit;
	}

	mMap.clear();
}