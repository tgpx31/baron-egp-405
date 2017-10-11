#include <stdio.h>
#include <assert.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <sstream>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "KinematicUnit.h"
#include "PlayerMoveToMessage.h"



Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;

Game::Game()
	:mpGraphicsSystem(NULL)
	, mpGraphicsBufferManager(NULL)
	, mpSpriteManager(NULL)
	, mpLoopTimer(NULL)
	, mpMasterTimer(NULL)
	, mShouldExit(false)
	, mpSample(NULL)
	, mBackgroundBufferID(INVALID_ID)
	, mpUnitManager(NULL)
	, mpUnitManager2(NULL)
	, mpInputManager(NULL)
	, mpWallManager(NULL)
	, mpUI(NULL)
	, mReactionRadius(150)
	, mMaxAcceleration(150.0f)
	, mMaxVelocity(200.0f)
	, mMaxAngularVelocity(25.0f)
	, mDebug(false)
	, mCohesionWeight(5)
	, mSeparationWeight(2)
	, mAlignmentWeight(5)
	, mVelocityMatchingWeight(2)
	, mDataMode(0)
	, mIsHost(false)
{
}

Game::~Game()
{
	//cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//startup allegro
	if(!al_init()) 
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return false;
	}

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager();
	mpSpriteManager = new SpriteManager();
	mpInputManager = new InputManager();

	//startup a lot of allegro stuff

	//load image loader addon
	if( !al_init_image_addon() )
	{
		fprintf(stderr, "image addon failed to load!\n");
		return false;
	}

	//install audio stuff
	if( !al_install_audio() )
	{
		fprintf(stderr, "failed to initialize sound!\n");
		return false;
	}

	if(!al_init_acodec_addon())
	{
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return false;
	}
 
	if (!al_reserve_samples(1))
	{
		fprintf(stderr, "failed to reserve samples!\n");
		return false;
	}

	//should be somewhere else!
	al_init_font_addon();
	if (!al_init_ttf_addon())
	{
		printf("ttf font addon not initted properly!\n");
		return false;
	}

	//actually load the font
	mpFont = al_load_ttf_font("cour.ttf", 20, 0);
	if (mpFont == NULL)
	{
		printf("ttf font file not loaded properly!\n");
		return false;
	}

	//InputManager initializes allegro input stuff
	if (!mpInputManager->init())
	{
		return false;
	}
	mpInputManager->setFont(mpFont);

	if( !al_init_primitives_addon() )
	{
		printf( "Primitives addon not added!\n" ); 
		return false;
	}

	//load the sample
	mpSample = al_load_sample( "clapping.wav" );
	if (!mpSample)
	{
		printf( "Audio clip sample not loaded!\n" ); 
		return false;
	}

	mpMessageManager = new GameMessageManager();

	//load buffers
	mBackgroundBufferID = mpGraphicsBufferManager->loadBuffer("wallpaper.bmp");
	mPlayerIconBufferID = mpGraphicsBufferManager->loadBuffer("arrow.bmp");
	mEnemyIconBufferID = mpGraphicsBufferManager->loadBuffer("enemy-arrow.bmp");
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, pPlayerBuffer->getWidth(), pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer( mEnemyIconBufferID );
	Sprite* pEnemyArrow = NULL;
	if( pAIBuffer != NULL )
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite( AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, pAIBuffer->getWidth(), pAIBuffer->getHeight() );
	}

	//setup UnitManager
	mpUnitManager = new UnitManager(pEnemyArrow);

	// Unit manager for Data Sharing
	mpUnitManager2 = new UnitManager(pEnemyArrow);
	
	/*
	Vector2D pos(500.0f, 500.0f);
	Vector2D vel(0.0f, 0.0f);
	mpUnitManager->createUnit(PLAYER_ID, pArrowSprite, pos, 1, vel, 0.0f, mMaxVelocity, mMaxAcceleration);
	*/

	mpUI = new UI(mpFont, Vector2D());

	mpWallManager = new WallManager();
	mpWallManager->createWall(Vector2D(0, 0), Vector2D(1024, 32));
	mpWallManager->createWall(Vector2D(0, 0), Vector2D(32, 768));
	mpWallManager->createWall(Vector2D(992, 0), Vector2D(1024, 768));
	mpWallManager->createWall(Vector2D(0, 736), Vector2D(1024, 768));

	return true;
}

void Game::cleanup()
{
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpUnitManager2;
	mpUnitManager2 = NULL;

	delete mpInputManager;
	mpInputManager = NULL;

	delete mpUI;
	mpUI = NULL;

	delete mpWallManager;
	mpWallManager = NULL;

	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;

	al_destroy_sample(mpSample);
	mpSample = NULL;

	al_destroy_font(mpFont);
	mpFont = NULL;

	//shutdown components
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	al_uninstall_audio();
	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}
	
void Game::processLoop()
{
	// If client on data push, do not do any updates
	if (!(mDataMode == 1 && !mIsHost))
	{
		mpInputManager->update(); 

		//update units
		mpUnitManager->update(LOOP_TARGET_TIME / 1000.0f); 
	}
	else if (mDataMode == 2)	// if data sharing
	{
		// each peer should simulate their own 
		// sends change in state to the other
		// use mpUnitManager for your own flock, update mpUnitManager2 for the other flock
		mpInputManager->update();

		//update units
		mpUnitManager->update(LOOP_TARGET_TIME / 1000.0f);
		//mpUnitManager2->update(LOOP_TARGET_TIME / 1000.0f);
	}
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	pBackgroundSprite->draw(*(mpGraphicsSystem->getBackBuffer()), 0, 0);

	//draw units
	mpUnitManager->draw(GRAPHICS_SYSTEM->getBackBuffer(), mDebug);
	mpUnitManager2->draw(GRAPHICS_SYSTEM->getBackBuffer(), mDebug);
	mpWallManager->draw(mDebug);
	//mpInputManager->draw();

	if (mDebug)
		mpUI->draw(); 

	mpMessageManager->processMessagesForThisframe();
	mpGraphicsSystem->swap();
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}