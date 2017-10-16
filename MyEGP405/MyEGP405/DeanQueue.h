#ifndef _DEAN_QUEUE_H_
#define _DEAN_QUEUE_H_


class Event;

class DeanQueue
{
private:
	static const unsigned int MAX_ITEMS_IN_QUEUE = 255;
	Event* mQueue[MAX_ITEMS_IN_QUEUE] = { 0 };

	typedef int size_type;
	size_type mCount = 0;

public:
	// Erasing/Reseting
	void Clear();
	void FillWith(Event aEvent);
	void Erase(size_type index = 0, size_type deleteCount = 1);

	// Add to the Queue
	void Reserve(const size_type amount);

	void Insert(size_type index, Event* aEvent);
	void Append(Event* aEvent);
	void Prepend(Event* aEvent);

	// Accessors
	inline size_type Size() const { return mCount; };

	inline const Event** Begin() { return &mQueue[0]; };
	inline const Event** End() { return mQueue + ((mCount + 1) * sizeof(Event*)); };

	inline Event* operator[](size_type index) { return mQueue[index]; };
};

#endif // _DEAN_QUEUE_H_