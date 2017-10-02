#pragma once

#include "Steering.h"
#include "KinematicWanderSteering.h"
#include "DynamicSeekSteering.h"
#include "SeparationSteering.h"

class DynamicWanderAndSeekSteering :public Steering
{
public:
	DynamicWanderAndSeekSteering(KinematicUnit* pMover, KinematicUnit* pTarget, bool shouldFlee);
	~DynamicWanderAndSeekSteering();

	void setTarget(KinematicUnit* pTarget) { mpSeekSteering->setTarget(pTarget); }
	
	virtual Steering* getSteering();

private:
	KinematicUnit* mpMover;
	KinematicUnit* mpTarget;
	DynamicSeekSteering* mpSeekSteering;
	KinematicWanderSteering* mpWanderSteering;
	SeparationSteering* mpSeparationSteering;
};