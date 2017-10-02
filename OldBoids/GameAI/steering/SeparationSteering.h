#pragma once

#include "Steering.h"
#include "KinematicUnit.h"
#include <vector>

class SeparationSteering : public Steering
{
	private:
		KinematicUnit* mpMover;
		int mRadius;

	public:
		SeparationSteering(KinematicUnit* pMover, int radius);
		~SeparationSteering();

		Steering* getSteering(std::vector<KinematicUnit*> targets);
};