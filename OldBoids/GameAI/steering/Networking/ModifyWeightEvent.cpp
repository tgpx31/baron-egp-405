//#include "ModifyWeightEvent.h"
//
//// ModifyWeightEvent ------------------------------------
//ModifyWeightEvent::ModifyWeightEvent(const unsigned int messageID, const unsigned int steeringType, const unsigned int newWeight)
//{
//	id = messageID;
//	steeringID = steeringType;
//	weight = newWeight;
//}
//
//void ModifyWeightEvent::Execute()
//{
//	switch (steeringID)
//	{
//		case 1:
//		{
//			gpGame->setCohesionWeight(weight);
//			break;
//		}
//
//		case 2:
//		{
//			gpGame->setSeparationWeight(weight);
//			break;
//		}
//
//		case 3:
//		{
//			gpGame->setAlignmentWeight(weight);
//			break;
//		}
//
//		default:
//		{
//			printf("Error: invalid steering identifier %d\n", steeringID);
//			break;
//		}
//
//	}
//}
//
//// !ModifyWeightEvent -----------------------------------