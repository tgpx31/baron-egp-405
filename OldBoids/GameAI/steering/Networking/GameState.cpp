// Brian Baron		0974390
// Justin Mulkin	0952465
//
// EGP 405-02	Lab 2	2017/09/25	(YYYY/MM/DD)
//
//
//--------------------------------------------------------------------------------------------------------------------------------
//		 We certify that this work is entirely our own.The assessor of this project may reproduce this project
//		 and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism-checking service,
//		 which may retain a copy of the project on its database
//--------------------------------------------------------------------------------------------------------------------------------
//
// Repository Link: https://github.com/tgpx31/baron-egp-405
//

// This file wasmodified by Brian Baron, Colin Brady, and Justin Mulkin with permission from author.

#include "GameState.h"

const int WIDTH = 1024;
const int HEIGHT = 768;
const float LOOP_TARGET_TIME = 33.3f;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;

const std::string PLAYER_ID = "player";
const std::string ENEMY_ID = "enemy";

void GameState::init(State * prev = nullptr, State ** currentState = nullptr)
{
	if (currentState != nullptr)
		State::init(currentState);

	mData.doesUpdateInput = 1;
	mData.doesUpdateState = 1;
	mData.doesUpdateNetworking = 0;
	mData.doesDisplay = 1;

	if (prev != nullptr)
		mPrev = prev;

	strcpy(mData.promptBuffer, "MyGame\n");

	initialized = 0;
	exit = 0;
}

void GameState::updateInput()
{
	//Get Input
	al_get_mouse_state(&mGameState.mCurrentMouseState);
	al_get_keyboard_state(&mGameState.mCurrentKeyboardState);

	// Exit
	if (keyPressed(ALLEGRO_KEY_ESCAPE))
	{
		// Stop updating
		mData.doesUpdateInput = mData.doesUpdateState = mData.doesUpdateNetworking = mData.doesDisplay = 0;
		// Cleanup
		cleanup();
		// Exit state to lobby
		// for now exit everything
		mData.running = 0;
		return;
	}

	// Movement
	if (keyPressed(ALLEGRO_KEY_A))
	{
		// walk left
		mGameState.mpLocalPlayer->setXVelocity(-1);
	}
	else if (keyUp(ALLEGRO_KEY_A))
	{
		mGameState.mpLocalPlayer->setXVelocity(0);
	}
	if (keyPressed(ALLEGRO_KEY_D))
	{
		// walk right
		mGameState.mpLocalPlayer->setXVelocity(+1);
	}
	else if (keyUp(ALLEGRO_KEY_D))
	{
		mGameState.mpLocalPlayer->setXVelocity(0);
	}

	mGameState.mPreviousKeyboardState = mGameState.mCurrentKeyboardState;
	mGameState.mPreviousMouseState = mGameState.mCurrentMouseState;
}

void GameState::updateData()
{
	if (exit)
		mData.running = 0;

	mGameState.mpLocalPlayer->update(LOOP_TARGET_TIME / 1000.0f);
}

void GameState::processBuffer()
{
	clearBuffer();
}

void GameState::render()
{
	if (!initialized)
		return;

	// draw bg
	Sprite* pBackgroundSprite = mGameState.mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	pBackgroundSprite->draw(*(mGameState.mpGraphicsSystem->getBackBuffer()), 0, 0);

	// draw your player
	mGameState.mpLocalPlayer->draw(mGameState.mpGraphicsSystem->getBackBuffer());

	// flip the buffers
	mGameState.mpGraphicsSystem->swap();
}

void GameState::ArriveFromPreviousState(StateData *data)
{
	init();
	mData.doesDisplay = 1;
	initialized = initGame();
}

int GameState::initGame()
{
	mGameState.mExit = 0;
	mGameState.mpLoopTimer = new Timer;

	if (!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	//create and init GraphicsSystem
	mGameState.mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mGameState.mpGraphicsSystem->init(WIDTH, HEIGHT);
	if (!goodGraphics)
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return -1;
	}

	mGameState.mpGraphicsBufferManager = new GraphicsBufferManager();
	mGameState.mpSpriteManager = new SpriteManager();

	//load image loader addon
	if (!al_init_image_addon())
	{
		fprintf(stderr, "image addon failed to load!\n");
		return -1;
	}

	al_init_font_addon();
	if (!al_init_ttf_addon())
	{
		printf("ttf font addon not initted properly!\n");
		return -1;
	}

	//actually load the font
	mGameState.mpFont = al_load_ttf_font("cour.ttf", 20, 0);
	if (mGameState.mpFont == NULL)
	{
		printf("ttf font file not loaded properly!\n");
		return -1;
	}

	// Input setup
	if (!al_install_keyboard())
	{
		printf("Keyboard not installed!\n");
		return -1;
	}

	if (!al_install_mouse())
	{
		printf("Mouse not installed!\n");
		return -1;
	}

	if (!al_init_primitives_addon())
	{
		printf("Primitives addon not added!\n");
		return -1;
	}

	//load buffers
	mGameState.mBackgroundBufferID = mGameState.mpGraphicsBufferManager->loadBuffer("wallpaper.bmp");
	mGameState.mPlayerIconBufferID = mGameState.mpGraphicsBufferManager->loadBuffer("arrow.bmp");
	mGameState.mEnemyIconBufferID = mGameState.mpGraphicsBufferManager->loadBuffer("enemy-arrow.bmp");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mGameState.mpGraphicsBufferManager->getBuffer(mGameState.mBackgroundBufferID);
	if (pBackGroundBuffer != NULL)
	{
		mGameState.mpSpriteManager->createAndManageSprite(BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, pBackGroundBuffer->getWidth(), pBackGroundBuffer->getHeight());
	}

	GraphicsBuffer* pPlayerBuffer = mGameState.mpGraphicsBufferManager->getBuffer(mGameState.mPlayerIconBufferID);
	Sprite* pArrowSprite = NULL;
	if (pPlayerBuffer != NULL)
	{
		pArrowSprite = mGameState.mpSpriteManager->createAndManageSprite(PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, pPlayerBuffer->getWidth(), pPlayerBuffer->getHeight());
	}

	GraphicsBuffer* pAIBuffer = mGameState.mpGraphicsBufferManager->getBuffer(mGameState.mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mGameState.mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, pAIBuffer->getWidth(), pAIBuffer->getHeight());
	}

	//****TODO: Units
	Vector2D playerStartPos;
	playerStartPos.setX(WIDTH/2);
	playerStartPos.setY(HEIGHT - pArrowSprite->getHeight());
	mGameState.mpLocalPlayer = new Player(pArrowSprite, playerStartPos, 0, 1);

	return 1;
}
int GameState::cleanup()
{
	//****TODO: delete units
	delete mGameState.mpLocalPlayer;
	mGameState.mpLocalPlayer = nullptr;

	// uninstall input
	al_uninstall_keyboard();
	al_uninstall_mouse();

	delete mGameState.mpLoopTimer;
	mGameState.mpLoopTimer = nullptr;

	delete mGameState.mpGraphicsSystem;
	mGameState.mpGraphicsSystem = nullptr;

	delete mGameState.mpGraphicsBufferManager;
	mGameState.mpGraphicsBufferManager = nullptr;
	delete mGameState.mpSpriteManager;
	mGameState.mpSpriteManager = nullptr;

	al_destroy_font(mGameState.mpFont);
	mGameState.mpFont = nullptr;

	//shutdown components
	al_shutdown_font_addon();
	al_shutdown_ttf_addon();
	
	al_shutdown_image_addon();
	al_shutdown_primitives_addon();

	return 1;
}
