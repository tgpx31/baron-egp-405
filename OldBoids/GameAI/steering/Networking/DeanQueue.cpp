#include "DeanQueue.h"

void DeanQueue::Clear()
{
	// reset size to 0, stored data is out of bounds and doesn't matter
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
	// Make sure the deletions are within the queue
	if (&mQueue[index] + (deleteCount * sizeof(Event*)) < End())
	{
		for (unsigned int i = index; i < index + deleteCount; ++i)
		{
			// If there are events after the deleted one, move them up
			if (&mQueue[i + deleteCount] < End())
			{
				mQueue[i] = mQueue[i + deleteCount];
			}
			else
			{
				mQueue[i] = 0;
			}
		}

		// reset bounds size
		mCount -= deleteCount;
	}
	// else if not in queue bounds...
	else
	{
		printf("ERROR: Index %i is out of bounds.\n", index);
	}
}

void DeanQueue::Reserve(const size_type amount)
{
	// if shrinking the queue...
	if (mCount > amount && amount >= 0)
	{
		mCount = amount;
		printf("Queue shrunk for %i Events\n", mCount);
	}
	// else if increase size, over maximum...
	else if (amount > mCount && amount >= MAX_ITEMS_IN_QUEUE)
	{
		// Clamp at the maximum
		mCount = MAX_ITEMS_IN_QUEUE;
		printf("Queue set to MAX_LENGTH %i\n", mCount);
	}
	// else if increase in bounds...
	else if (amount > mCount && amount < MAX_ITEMS_IN_QUEUE)
	{
		mCount = amount;
		printf("Queue enlarged for %i Events\n", mCount);
	}
	// else, invalid size
	else
	{
		printf("ERROR: Invalid size. Must be a positive value\nQueue length: %i\n", mCount);
	}
}

void DeanQueue::Insert(size_type index, Event * aEvent)
{
	// if no room...
	if (Size() >= MAX_ITEMS_IN_QUEUE)
	{
		printf("ERROR: No room to insert in queue\n");
		return;
	}
	// else there is room...
	else
	{
		// Create a tmp array to make sure data isn't overwritten
		unsigned int i;
		Event** tmp = (Event**)malloc(sizeof(Event*) * (Size() - index));

		for (i = index; i < Size(); ++i)
		{
			*(tmp + (sizeof(Event*) * i)) = mQueue[i+1];
		}

		// Set the new data
		mQueue[index] = aEvent;
		Reserve(index);
		// Add tmp array back into the data
		for (i = 0; i < Size() - 1 - index; ++i)
		{
			Append(*(tmp + (sizeof(Event*) * i)));
		}

		// Deallocate the temp array
		free(tmp);
	}
}

void DeanQueue::Append(Event * aEvent)
{
	// Verify room to allocate
	if (End() <= mQueue + (MAX_ITEMS_IN_QUEUE * sizeof(Event*)))
	{
		mQueue[mCount] = aEvent;
		++mCount;
	}
	else
	{
		printf("ERROR: No room to append\n");
	}
}

void DeanQueue::Prepend(Event * aEvent)
{
	Insert(0, aEvent);
}

Event * DeanQueue::Pop()
{
	// grab the value, move the rest of the queue up an index

	Event *ev = operator[](0);
	Erase(0);
	return ev;
}

void DeanQueue::ExecuteAllEvents()
{
	// iterate through the list of events, run all execute functions, pop from list
	while (Size() > 0)
	{
		Event* ev = Pop();
		ev->Execute();
	}
}


