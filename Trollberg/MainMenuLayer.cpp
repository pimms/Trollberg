#pragma once 
#include "Pim.h"
#include "MainMenuLayer.h"
#include "ButtonScroller.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "ParallaxLayer.h"
#include "MenuButton.h"

MainMenuLayer::MainMenuLayer()
{
	buttonSheet	= NULL;
	scrollSheet = NULL;

	startGame	= false;
	
	isScrolling = true;
	position.x  = -2800.f;
	scrollDest	= -1080.f;
	numScrolls	= 0;

	startLVL	= 0;
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
	music = new Pim::Sound("songz\\MENU.ogg");
	music->loop();

	// Create the lighting system
	createLightingSystem(Pim::Vec2(1920.f, 1080.f));
	setLightingUnlitColor(Pim::Color(0.f, 0.f, 0.f, 0.85f));

	// Setup a light def
	Pim::FlatLightDef *ld = new Pim::FlatLightDef;
	ld->radius		= 300;
	ld->falloff		= 2.f;
	ld->innerColor  = Pim::Color(1.f, 1.f, 0.f, 1.f);
	
	// Create the light
	light = new Pim::GameNode;
	addChild(light);
	addLight(light, ld);


	listenFrame();
	listenKeys();
}
void MainMenuLayer::loadButtons()
{
	//std::string menyButtonLabels[NUMMENYBUTTONS]  = {"Start", "Options", "Highscore", "Exit"};
	const char *labels[]  = {
		"Start lvl1", 
		"Start lvl2", 
		"Start lvl3", 
		"Credits"
	};

	buttonScroller = new ButtonScroller(buttonSheet, 4, labels, this);
	buttonScroller->position.x = 1080.f + 192.f;
	buttonScroller->setZOrder(2);
	addChild(buttonScroller);
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

void MainMenuLayer::buttonPressed(Pim::Button* activeButton)
{
	int idx = buttonScroller->getButtonID(activeButton);

	if (idx == 0)
	{
		startLVL = 1;
		startGame = true;
		buttonScroller->scrollUp();
	}
	else if (idx == 1)
	{
		startLVL = 2;
		startGame = true;
		buttonScroller->scrollUp();
	}
	else if (idx == 2)
	{
		startLVL = 3;
		startGame = true;
		buttonScroller->scrollUp();
	}
	else if (idx == 3)
	{
		exit(0);
	}
}

void MainMenuLayer::update(float dt)
{
	if (startGame && buttonScroller->doneScrolling())
	{
		isScrolling = true;
		scrollDest = -20.f;
		startGame = false;
	}

	updateScroll(dt);
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
		light->position = Pim::Vec2(192.f-position.x, 108 - 50.f * sinf(position.x/100.f));

		if (position.x > scrollDest)
		{
			position.x	= scrollDest;
			isScrolling = false;
			numScrolls++;

			if (numScrolls == 1)
			{
				// Scroll down the buttons
				buttonScroller->scrollDown();
			}
			else
			{
				// Go to game
				Pim::GameControl::getSingleton()->setScene(new GameScene(startLVL));
			}
		}
	}
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