#include "State.h"
#include "Timer.h"

int main()
{
	State ApplicationState[1];
	Timer timer;

	const float framerate = 1.0f / 30.0f;

	ApplicationState->mData.running = 1;
	
	while (ApplicationState->mData.running)
	{
		timer.start();

		ApplicationState->update();
		ApplicationState->render();

		timer.sleepUntilElapsed(framerate * 1000);
		timer.stop();
	}

	return 0;
}