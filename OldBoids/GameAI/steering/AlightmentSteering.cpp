#include "AlightmentSteering.h"

AlignmentSteering::AlignmentSteering(KinematicUnit* pMover, int radius)
{
	mpMover = pMover;
	mRadius = radius;
}

AlignmentSteering::~AlignmentSteering()
{
}

Steering* AlignmentSteering::getSteering(std::vector<KinematicUnit*> targets)
{
	float averageAngular = 0;
	int count = 0;
	std::vector<KinematicUnit*>::iterator iter;

	for (iter = targets.begin(); iter != targets.end(); iter++)
	{
		KinematicUnit* target = *iter;
		Vector2D distance = target->getPosition() - mpMover->getPosition();
		if (distance.getLength() <= mRadius)
		{
			averageAngular += target->getRotation();
			count++;
		}
	}

	mLinear = 0;
	mAngular = 0;

	if (count)
	{
		averageAngular /= count;
		mAngular = averageAngular;
	}
		
	return this;
}