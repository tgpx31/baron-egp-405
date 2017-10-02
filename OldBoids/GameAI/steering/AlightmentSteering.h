#pragma once

#include "Steering.h"
#include "KinematicUnit.h"
#include <vector>

class AlignmentSteering : public Steering
{
	private:	
		KinematicUnit* mpMover;
		int mRadius;

	public:
		AlignmentSteering(KinematicUnit* pMover, int radius);
		~AlignmentSteering();

		Steering* getSteering(std::vector<KinematicUnit*> targets);

};