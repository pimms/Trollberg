#pragma once 
#include "Pim.h"
#include "MainMenuLayer.h"
#include "ButtonScroller.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "ParallaxLayer.h"
#include "MenuButton.h"

/*
	Two ButtonScrollers are utilized - mainScroller and playScroller. Once a button in
	either of them has been clicked the scrollID value is set to corespond to the function
	of the button, and once the buttonScroller has scrolled to the top, the scrollID's value
	is used to determine what action will be taken next.
*/

MainMenuLayer::MainMenuLayer()
{
	music				= NULL;

	buttonSheet			= NULL;
	scrollSheet			= NULL;

	startGame			= false;
	
	isScrolling			= true;
	position.x			= -2800.f;
	scrollDest			= -1080.f;
	numScrolls			= 0;

	activeScroller		= NULL;
	buttonsScrolling	= false;
	scrollID			= SHOW_MAIN;

	startLVL			= 0;
}

MainMenuLayer::~MainMenuLayer()
{
	if (music)
	{
		delete music;
	}
}

void MainMenuLayer::loadResources()
{
	// Load the button sheet
	buttonSheet = new Pim::SpriteBatchNode("res/mainMenuButtons.png");
    addChild(buttonSheet);

	loadButtons();

	// Load the scroll sheet
	scrollSheet = new Pim::SpriteBatchNode("res/mainmenu.png");
	addChild(scrollSheet);
	loadParallax();

	// Load the middle layer (this)
	loadSprites();

	// Load the music file
	music = new Pim::Sound("res\\menumusic.ogg");
	music->loop();

	// Create the lighting system
	createLightingSystem(Pim::Vec2(1920.f, 1080.f));
	setLightingUnlitColor(Pim::Color(0.f, 0.f, 0.f, 0.85f));

	// Setup a light def
	Pim::FlatLightDef *ld = new Pim::FlatLightDef;
	ld->radius		= 90;
	ld->falloff		= 2.f;
	ld->innerColor  = Pim::Color(1.f, 1.f, 0.f, 1.f);
	
	// Create the light
	light = new Pim::GameNode;
	light->position = Pim::Vec2(1080.f+192.f, 90.f);
	addChild(light);
	addLight(light, ld);

	// Load the rain
	loadRain();

	listenFrame();
	listenKeys();
}
void MainMenuLayer::loadButtons()
{
	const char *mainLabels[]  = {
		"Play",
		"Highscores",
		"Exit"
	};
	mainScroller = new ButtonScroller(buttonSheet, 3, mainLabels, this);
	mainScroller->position.x = 1080.f + 192.f;
	mainScroller->setZOrder(2);
	mainScroller->hidden = true;
	addChild(mainScroller);

	const char *playLabels[]  = {
		"Level 1",
		"Level 2",
		"Level 3",
		"Back",
	};
	playScroller = new ButtonScroller(buttonSheet, 4, playLabels, this);
	playScroller->position.x = 1080.f + 192.f;
	playScroller->position.y = 150.f;
	playScroller->setZOrder(2);
	playScroller->hidden = true;
	addChild(playScroller);
}
void MainMenuLayer::loadSprites()
{
	for (int i=0; i<6; i++)
	{
		int origX = 512+512*(i%3);
		int origY = 240+240*(i/3);

		Pim::Sprite *sprite = new Pim::Sprite;
		sprite->anchor		= Pim::Vec2(0.f, 0.f);
		sprite->position.x	= 512 * i;
		sprite->rect		= Pim::Rect(origX, origY, 512, 240);
		sprite->useBatchNode(scrollSheet);
		addChild(sprite);
	}
}
void MainMenuLayer::loadParallax()
{
	// Load the backmost layer (the mountains)
	ParallaxLayer *back = new ParallaxLayer(this, 0.5f);
	back->setZOrder(-2);
	parentScene->addLayer(back);

	for (int i=0; i<3; i++)
	{
		Pim::Sprite *sprite = new Pim::Sprite;
		sprite->anchor		= Pim::Vec2(0.f, 0.f);
		sprite->position	= Pim::Vec2(512*i+180, 0.f);
		sprite->rect		= Pim::Rect(512+512*i, 0, 512, 240);
		sprite->useBatchNode(scrollSheet);
		back->addChild(sprite);
	}

	// Load the frontmost laster (the forest)
	ParallaxLayer *front = new ParallaxLayer(this, 1.5f);
	front->setZOrder(2);
	parentScene->addLayer(front);

	for (int i=0; i<8; i++)
	{
		Pim::Sprite *sprite = new Pim::Sprite;
		sprite->anchor		= Pim::Vec2(0.f, 0.0f);
		sprite->position	= Pim::Vec2(4096.f-512*i, 0.f);
		sprite->rect		= Pim::Rect(0, 240*i, 512, 240);
		sprite->useBatchNode(scrollSheet);
		front->addChild(sprite);
	}

	// Add the first sprite again to create all black on both extremes
	Pim::Sprite *sprite = new Pim::Sprite;
	sprite->anchor		= Pim::Vec2(0.f, 0.f);
	sprite->position	= Pim::Vec2(0.f, 0.f);
	sprite->rect		= Pim::Rect(0, 0, 512, 240);
	sprite->useBatchNode(scrollSheet);
	front->addChild(sprite);
}
void MainMenuLayer::loadRain()
{
	for (int i=0; i<1000; i++)
	{
		Pim::Sprite *r = new Pim::Sprite;
		r->position = Pim::Vec2(-position.x-450+rand()%835, rand()%230);
		r->rect = Pim::Rect(1024+rand()%4,721,1,8);
		r->useBatchNode(scrollSheet);

		scrollSheet->addChild(r);
		rain.push_back(r);
	}
}

void MainMenuLayer::buttonPressed(Pim::Button* activeButton)
{
	int idx = mainScroller->getButtonID(activeButton);
	if (idx != -1)
	{
		activeScroller = mainScroller;
		mainScroller->scrollUp();

		if (idx == 0)
		{
			scrollID = SHOW_PLAY;
		}
		else if (idx == 1)
		{
			scrollID = SHOW_HIGHSCORE;
		}
		else if (idx == 2)
		{
			scrollID = EXIT;
		}
	}

	idx = playScroller->getButtonID(activeButton);
	if (idx != -1)
	{
		startLVL = idx + 1;

		// Go straight to the game
		if (idx == 0)
		{
			startGame = true;
			isScrolling = true;
			scrollDest = -20.f;
		}
		else if (idx == 1)
		{
			startGame = true;
			isScrolling = true;
			scrollDest = -20.f;
		}
		else if (idx == 2)
		{
			startGame = true;
			isScrolling = true;
			scrollDest = -20.f;
		}

		// Scroll back the main buttons
		else if (idx == 3)
		{
			activeScroller = playScroller;
			playScroller->scrollUp();
			scrollID = SHOW_MAIN;
		}
	}
}

void MainMenuLayer::update(float dt)
{
	if (activeScroller && activeScroller->doneScrolling())
	{
		scrollCompleted();
	}

	// Fade the music as we're leaving the scene
	music->setVolume(((position.x)-500)/-1000.f);

	updateRain(dt);

	updateScroll(dt);
}
void MainMenuLayer::updateRain(float dt)
{
	float d = 0.f;

	for each (Pim::Sprite *r in rain)
	{
		d = r->position.x + position.x;
		r->position.y -= 180 * dt;

		if (r->position.y < 0.f)
		{
			r->position.y += 230.f;
			r->position.x = -position.x - 450 + rand()%835;
		}
	}
}
void MainMenuLayer::updateScroll(float dt)
{
	// STARTPOS: -2800, 0
	// MENUPOS: -1080, 0
	// ENDPOS: -20, 0

	if (isScrolling)
	{
		//float diff = pow(position.x + scrollDest,2);
		float x = abs(position.x - scrollDest);
		float diff = log((x+1)/10.f) * 150.f;

		if (diff > 700.f)
		{
			diff = 700.f;
		}
		else if (diff < 15.f)
		{
			diff = 15.f;
		}

		position.x += diff*dt*MENYSPEED;

		if (position.x > scrollDest)
		{
			position.x	= scrollDest;
			isScrolling = false;
			numScrolls++;

			if (numScrolls == 1)
			{
				// Scroll down the buttons
				mainScroller->scrollDown();
			}
			else
			{
				// Go to game
				((MainMenuScene*)parentScene)->startGame(startLVL);
			}
		}
	}
}

void MainMenuLayer::scrollCompleted()
{
	if (scrollID == SHOW_MAIN)
	{
		playScroller->position.y = 150.f;
		mainScroller->position.y = 0.f;

		mainScroller->scrollDown();
	}
	else if (scrollID == SHOW_PLAY)
	{
		playScroller->position.y = 0.f;
		mainScroller->position.y = 150.f;

		playScroller->scrollDown();
	}
	else if (scrollID == SHOW_HIGHSCORE)
	{
	}
	else if (scrollID == PLAY)
	{
		startGame = true;
		isScrolling = true;
		scrollDest = -20.f;
	}
	else if (scrollID == EXIT)
	{
		exit(0);	// Sayanora
	}

	scrollID = NONE;
	activeScroller = NULL;
}

/* DEBUG */
void MainMenuLayer::keyEvent(Pim::KeyEvent &evt)
{
	if (evt.isKeyDown(Pim::KeyEvent::K_D))
	{
		position.x -= 20;
	}
	if (evt.isKeyDown(Pim::KeyEvent::K_A))
	{
		position.x += 20;
	}
	if (evt.isKeyFresh(Pim::KeyEvent::K_P))
	{
		printf("MLPOS: %0.1f, %0.1f\n", position.x, position.y);
	}
}