#pragma once

#include "Steering.h"
#include "KinematicUnit.h"
#include <vector>

class CohesionSteering : public Steering
{
	private:
		KinematicUnit* mpMover;
		int mRadius;

	public:
		CohesionSteering(KinematicUnit* pMover, int radius);
		~CohesionSteering();

		Steering* getSteering(std::vector<KinematicUnit*> targets);
};