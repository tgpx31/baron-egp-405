#pragma once

#include "Steering.h"
#include "KinematicUnit.h"
#include <vector>

class VelocityMatchingSteering : public Steering
{
	private:
		KinematicUnit* mpMover;
		int mRadius;

	public:
		VelocityMatchingSteering(KinematicUnit* pMover, int radius);
		~VelocityMatchingSteering();

		Steering* getSteering(std::vector<KinematicUnit*> targets);
};
