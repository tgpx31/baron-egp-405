#include "EditDebugValueMessage.h"
#include "Game.h"
#include "UI.h"

EditDebugValueMessage::EditDebugValueMessage(float value) : GameMessage(EDIT_DEBUG_VALUE_MESSAGE)
{
	mValue = value;
}

EditDebugValueMessage::~EditDebugValueMessage()
{
}

void EditDebugValueMessage::process()
{
	DebugMode mode = gpGame->getUI()->getMode();
	float workingValue;

	switch (mode)
	{
		case VELOCITY: 
			workingValue = gpGame->getMaxVelocity();
			workingValue += mValue;
			gpGame->setMaxVelocity(workingValue);
			gpGame->getLocalUnitManager()->updateMaxVelocity();
			break;
		case RADIUS:
			workingValue = gpGame->getReactionRadius();
			workingValue += mValue;
			gpGame->setReactionRadius(workingValue);
			break;
		case ACCELERATION:
			workingValue = gpGame->getMaxAcceleration();
			workingValue += mValue;
			gpGame->setMaxAcceleration(workingValue);
			gpGame->getLocalUnitManager()->updateMaxAcceleration();
			break;
		case ANGULAR:
			workingValue = gpGame->getMaxAngularVelocity();
			workingValue += mValue;
			gpGame->setMaxAngularVelocity(workingValue);
			gpGame->getLocalUnitManager()->updateMaxAngularVelocity();
			break;
		case COHESION:
			workingValue = gpGame->getCohesionWeight();
			workingValue += (mValue / 5.0f);
			gpGame->setCohesionWeight(workingValue);
			break;
		case SEPARATION:
			workingValue = gpGame->getSeparationWeight();
			workingValue += (mValue / 5.0f);
			gpGame->setSeparationWeight(workingValue);
			break;
		case ALIGNMENT:
			workingValue = gpGame->getAlignmentWeight();
			workingValue += (mValue / 5.0f);
			gpGame->setAlignmentWeight(workingValue);
			break;
		case VELOCITY_MATCHING:
			workingValue = gpGame->getVelocityMatchingWeight();
			workingValue += (mValue / 5.0f);
			gpGame->setVelocityMatchingWeight(workingValue);
		default: 
			break;
	}
}
