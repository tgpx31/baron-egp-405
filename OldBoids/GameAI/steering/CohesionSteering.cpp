#include "CohesionSteering.h"

CohesionSteering::CohesionSteering(KinematicUnit* pMover, int radius)
{
	mpMover = pMover;
	mRadius = radius;
}

CohesionSteering::~CohesionSteering()
{
}

Steering* CohesionSteering::getSteering(std::vector<KinematicUnit*> targets)
{
	Vector2D averagePoint(0, 0);
	int count = 0;
	std::vector<KinematicUnit*>::iterator iter;
	
	for (iter = targets.begin(); iter != targets.end(); iter++)
	{
		KinematicUnit* target = *iter;
		Vector2D distance = target->getPosition() - mpMover->getPosition();
		if (distance.getLength() <= mRadius)
		{
			averagePoint += target->getPosition();
			count++;
		}
	}

	mLinear = 0;
	mAngular = 0;

	if (count)
	{
		averagePoint /= count;
		averagePoint.normalize();
		mLinear = averagePoint;
	}
		
	return this;
}
