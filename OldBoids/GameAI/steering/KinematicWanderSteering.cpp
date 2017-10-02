#include "KinematicWanderSteering.h"
#include "KinematicUnit.h"
#include "Game.h"

KinematicWanderSteering::KinematicWanderSteering(KinematicUnit *pMover)
:mpMover(pMover)
{
	mApplyDirectly = false;
}

Steering* KinematicWanderSteering::getSteering()
{
	mLinear = mpMover->getOrientationAsVector() * mpMover->getMaxVelocity(); 
	//mAngular = mpMover->getOrientation() * ( genRandomBinomial() * MAX_WANDER_ROTATION );

	mAngular = 15.0f;

	std::printf("mLinear: (%f, %f) mAngular: %f\n", mLinear.getX(), mLinear.getY(), mAngular);

	return this;
}