#pragma once

#include "GameMessage.h"

class EditDebugValueMessage : public GameMessage
{
public:
	EditDebugValueMessage(float value);
	~EditDebugValueMessage();

	void process();

private:
	float mValue;
};