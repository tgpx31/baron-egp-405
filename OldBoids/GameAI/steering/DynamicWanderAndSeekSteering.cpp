#include "DynamicWanderAndSeekSteering.h"
#include "KinematicUnit.h"
#include "Game.h"


DynamicWanderAndSeekSteering::DynamicWanderAndSeekSteering(KinematicUnit* pMover, KinematicUnit* pTarget, bool shouldFlee)
{
	mpMover = pMover;
	mpTarget = pTarget;
	mpSeekSteering = new DynamicSeekSteering(pMover, pTarget, shouldFlee);
	mpWanderSteering = new KinematicWanderSteering(pMover);
	mpSeparationSteering = new SeparationSteering(pMover, 60);
}

DynamicWanderAndSeekSteering::~DynamicWanderAndSeekSteering()
{
	delete mpSeekSteering;
	delete mpWanderSteering;
	delete mpSeparationSteering;
}

Steering* DynamicWanderAndSeekSteering::getSteering()
{
	Vector2D distance = mpMover->getPosition() - mpTarget->getPosition();
	Steering* steering;

	if (distance.getLength() > gpGame->getReactionRadius())
	{
		steering = mpWanderSteering->getSteering();
	}
	else
	{
		steering = mpSeekSteering->getSteering();
	}

	//return mpSeparationSteering->getSteering(steering, mpMover, gpGame->getLocalUnitManager()->getEnemyUnits());
	//Fix this later
	return nullptr;
}