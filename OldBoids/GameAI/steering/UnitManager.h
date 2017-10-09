#pragma once

#include "KinematicUnit.h"
#include "Behaviors.h"
#include <map>
#include <string>
#include <vector>

typedef std::string UnitKey;

class UnitManager : public Trackable
{
private:
	std::map<UnitKey, KinematicUnit*> mMap;
	Sprite* mpEnemySprite;

public:
	UnitManager(Sprite* pEnemySprite);
	~UnitManager();

	//Add existing unit
	void addUnit(KinematicUnit* pUnit, const UnitKey& key);

	/*
	//Create unit and add it to the manager
	UnitKey createUnit(const UnitKey& key, Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D& velocity, float rotationVel, float maxVelocity = 1.0f, float maxAcceleration = 1.0f);
	*/

	//Create new unit, assign behavior
	void createUnit(const Vector2D& position, Behavior behavior);

	//Remove units by specific unit pointer, key, or randomly
	void removeUnit(KinematicUnit* pUnit);
	void removeUnit(const UnitKey& key);
	void removeRandomEnemy();
	void removeEndUnit();

	//Update new values for all units currently in the manager
	void updateMaxVelocity();
	void updateMaxAcceleration();
	void updateMaxAngularVelocity();

	//Get a pointer to a unit with a key
	KinematicUnit* getUnit(const UnitKey& key);

	//Get a list of all current enemy units
	std::vector<KinematicUnit*> getEnemyUnits();

	void update(float time);
	void draw(GraphicsBuffer* pBuffer, bool drawCollider);
	void clear();

	inline const int getCount() const { return mMap.size(); }
};