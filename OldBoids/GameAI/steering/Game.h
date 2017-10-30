#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include "UnitManager.h"
#include "InputManager.h"
#include "UI.h"
#include "WallManager.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <string>

class GraphicsSystem;
class GraphicsBuffer;
class GraphicsBufferManager;
class SpriteManager;
class KinematicUnit;
class GameMessageManager;
class Timer;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;

const std::string PLAYER_ID = "player";
const std::string ENEMY_ID = "enemy";

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

class Game :public Trackable
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void beginLoop();
	void processLoop();
	bool endLoop();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };

	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline ALLEGRO_FONT* getFont() const { return mpFont; };
	inline KinematicUnit* getPlayerUnit() { return mpLocalUnitManager->getUnit(PLAYER_ID); };//should be someplace else

	inline void endGame() { mShouldExit = true; };

	inline int getReactionRadius() { return mReactionRadius; };
	inline float getMaxVelocity() { return mMaxVelocity; };
	inline float getMaxAcceleration() { return mMaxAcceleration; };
	inline float getMaxAngularVelocity() { return mMaxAngularVelocity; };
	inline void setReactionRadius(int radius) { mReactionRadius = radius; };
	inline void setMaxVelocity(float velocity) { mMaxVelocity = velocity; };
	inline void setMaxAcceleration(float acceleration) { mMaxAcceleration = acceleration; };
	inline void setMaxAngularVelocity(float angularVelocity) { mMaxAngularVelocity = angularVelocity; };

	inline void toggleDebug() { mDebug = !mDebug; };
	inline bool getDebug() const { return mDebug; };

	inline UI* getUI() const { return mpUI; };
	inline UnitManager* getLocalUnitManager() { return mpLocalUnitManager; };
	inline UnitManager* getPeerUnitManager() { return mpPeerUnitManager; };
	inline WallManager* getWallManager() { return mpWallManager; };

	inline int getCohesionWeight() { return mCohesionWeight; };
	inline int getSeparationWeight() { return mSeparationWeight; };
	inline int getAlignmentWeight() { return mAlignmentWeight; };
	inline int getVelocityMatchingWeight() { return mVelocityMatchingWeight; };
	inline void setCohesionWeight(int weight) { mCohesionWeight = weight; };
	inline void setSeparationWeight(int weight) { mSeparationWeight = weight; };
	inline void setAlignmentWeight(int weight) { mAlignmentWeight = weight; };
	inline void setVelocityMatchingWeight(int weight) { mVelocityMatchingWeight = weight; };

	inline int getDataMode() { return mDataMode; };
	inline bool getIsHost() { return mIsHost; };
	inline void setDataMode(int dataMode) { mDataMode = dataMode; };
	inline void setIsHost(bool isHost) { mIsHost = isHost; };

	inline int getWeight(DebugMode behavior)
	{
		switch (getUI()->getMode())
		{
		case 1:
			return getCohesionWeight();
			break;
		case 2:
			return getSeparationWeight();
			break;

		case 3:
			return getAlignmentWeight();
			break;

		}
	};

private:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	GameMessageManager* mpMessageManager;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	bool mShouldExit;

	ALLEGRO_FONT* mpFont;
	ALLEGRO_SAMPLE* mpSample;
	IDType mBackgroundBufferID;
	IDType mPlayerIconBufferID;
	IDType mEnemyIconBufferID;
	
	UnitManager *mpLocalUnitManager, *mpPeerUnitManager;
	InputManager* mpInputManager;
	WallManager* mpWallManager;
	UI* mpUI;

	int mReactionRadius;
	float mMaxVelocity;
	float mMaxAcceleration;
	float mMaxAngularVelocity;
	bool mDebug;

	int mCohesionWeight;
	int mSeparationWeight;
	int mAlignmentWeight;
	int mVelocityMatchingWeight;

	int mDataMode; // 1 = data push, 2 = data sharing, 3 = data coupled, 0 is local
	bool mIsHost;
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;

