#include "SeparationSteering.h"
#include "Game.h"

SeparationSteering::SeparationSteering(KinematicUnit* pMover, int radius)
{
	mpMover = pMover;
	mRadius = radius;
}

SeparationSteering::~SeparationSteering()
{
}

Steering* SeparationSteering::getSteering(std::vector<KinematicUnit*> targets)
{
	Vector2D direction;
	float distance;
	float strength;
	std::vector<KinematicUnit*>::iterator iter;

	mLinear = 0;
	mAngular = 0;

	for (iter = targets.begin(); iter != targets.end(); iter++)
	{
		KinematicUnit* target = *iter;
		//direction = target->getPosition() - mpMover->getPosition();
		direction = mpMover->getPosition() - target->getPosition();
		distance = direction.getLength();

		if (distance <= mRadius)
		{

			//strength = gpGame->getMaxAcceleration() * (mRadius * distance) / mRadius;
			//direction.normalize();
			//mLinear = mLinear - (direction * strength);
			mLinear += direction;
		}
	}

	return this;
}
