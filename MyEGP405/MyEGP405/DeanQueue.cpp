#include "Event.h"
#include "DeanQueue.h"


void DeanQueue::Clear()
{
	// reset size to 0, stored data is garbage data
	mCount = 0;
}

void DeanQueue::FillWith(Event aEvent)
{
	for (Event* ev : mQueue)
	{
		*ev = aEvent;
	}
}

void DeanQueue::Erase(size_type index, size_type deleteCount)
{

}

void DeanQueue::Reserve(const size_type amount)
{
}
