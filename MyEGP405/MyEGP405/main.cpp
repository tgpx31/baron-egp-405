// Brian Baron	0974390
// EGP 405-02	Lab 3
//
//
//--------------------------------------------------------------------------------------------------------------------------------
//		 We certify that this work is entirely our own.The assessor of this project may reproduce this project
//		 and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism-checking service,
//		 which may retain a copy of the project on its database
//--------------------------------------------------------------------------------------------------------------------------------
//
//
// This file was modified by Brian Baron with permission from author
// (I put this because of the tutorial source code just in case)
// Since this was not a team effort, all code modified/written for this Lab was Brian Baron
//
// Repository Link: https://github.com/tgpx31/baron-egp-405

#include "DeanQueue.h"

int main()
{
	DeanQueue q;
	Event* pEvent = new Event;
	Event* pEv;

	q.Clear();
	printf("%i", q.Size());
	
	q.Reserve(500);
	q.Reserve(-1);
	q.Reserve(0);

	q.Push(pEvent);
	pEv = q.Pop();

	return 0;
}