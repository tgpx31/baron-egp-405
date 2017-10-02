#pragma once

#include "SeparationSteering.h"
#include "AlightmentSteering.h"
#include "VelocityMatchingSteering.h"
#include "CohesionSteering.h"
#include "KinematicWanderSteering.h"

class BoidSteering : public Steering
{
private:
	SeparationSteering* mpSeparationSteering;
	AlignmentSteering* mpAlignmentSteering;
	VelocityMatchingSteering* mpVelocityMatchingSteering;
	CohesionSteering* mpCohesionSteering;
	KinematicWanderSteering* mpWanderSteering;

	KinematicUnit* mpUnit;

public:
	BoidSteering(KinematicUnit* pTarget);
	~BoidSteering();

	Steering* getSteering();
};