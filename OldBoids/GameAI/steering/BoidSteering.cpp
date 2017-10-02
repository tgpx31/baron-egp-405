#include "BoidSteering.h"
#include "Game.h"

BoidSteering::BoidSteering(KinematicUnit* pTarget)
{
	mpSeparationSteering = new SeparationSteering(pTarget, 45);
	mpAlignmentSteering = new AlignmentSteering(pTarget, 100);
	mpVelocityMatchingSteering = new VelocityMatchingSteering(pTarget, 100);
	mpCohesionSteering = new CohesionSteering(pTarget, 75);
	mpWanderSteering = new KinematicWanderSteering(pTarget);

	mpUnit = pTarget;
}

BoidSteering::~BoidSteering()
{
	delete mpSeparationSteering;
	delete mpAlignmentSteering;
	delete mpVelocityMatchingSteering;
	delete mpCohesionSteering;
	delete mpWanderSteering;
}

Steering* BoidSteering::getSteering()
{
	//Steering* baseWander = mpWanderSteering->getSteering(); //base steering to add everything to
	
	//update all steerings to be blended
	Steering* cohesion = mpCohesionSteering->getSteering(gpGame->getUnitManager()->getEnemyUnits());
	Steering* separation = mpSeparationSteering->getSteering(gpGame->getUnitManager()->getEnemyUnits());
	Steering* veclocityMatching = mpVelocityMatchingSteering->getSteering(gpGame->getUnitManager()->getEnemyUnits());
	Steering* alignment = mpAlignmentSteering->getSteering(gpGame->getUnitManager()->getEnemyUnits());

	/*
	
	Good code goes here
	
	*/

	mLinear = (cohesion->getLinear() * gpGame->getCohesionWeight()) + (separation->getLinear() * gpGame->getSeparationWeight()) + (veclocityMatching->getLinear() * gpGame->getVelocityMatchingWeight()) + (alignment->getLinear() * gpGame->getAlignmentWeight());
	mAngular = (cohesion->getAngular() * gpGame->getCohesionWeight()) + (separation->getAngular() * gpGame->getSeparationWeight()) + (veclocityMatching->getAngular() * gpGame->getVelocityMatchingWeight()) + (alignment->getAngular() * gpGame->getAlignmentWeight());

	mLinear.normalize();
	mLinear *= gpGame->getMaxVelocity();

	return this;
}
