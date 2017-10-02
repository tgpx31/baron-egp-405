#include "VelocityMatchingSteering.h"

VelocityMatchingSteering::VelocityMatchingSteering(KinematicUnit* pMover, int radius)
{
	mpMover = pMover;
	mRadius = radius;
}

VelocityMatchingSteering::~VelocityMatchingSteering()
{
}

Steering * VelocityMatchingSteering::getSteering(std::vector<KinematicUnit*> targets)
{
	Vector2D averageVelocity(0,0);
	int count = 0;
	std::vector<KinematicUnit*>::iterator iter;

	for (iter = targets.begin(); iter != targets.end(); iter++)
	{
		KinematicUnit* target = *iter;
		Vector2D distance = target->getPosition() - mpMover->getPosition();
		if (distance.getLength() <= mRadius)
		{
			averageVelocity += target->getVelocity();
			count++;
		}
	}

	mLinear = 0;
	mAngular = 0;

	if (count)
	{
		averageVelocity /= count;
		mLinear = averageVelocity;
	}
	
	return this;
}
