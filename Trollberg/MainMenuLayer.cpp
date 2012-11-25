#pragma once 
#include "Pim.h"
#include "MenuButton.h"
#include "MainMenuLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "ParallaxLayer.h"
#include "CreditsObject.h"
#include "MenuButton.h"

MainMenuLayer::MainMenuLayer()
{
	buttonFont	= NULL;
	buttonSheet	= NULL;
	scrollSheet = NULL;
	creditsObject = NULL;

	playIntro	= false;
	playOutro	= false;
	startGame	= false;

	playCredits = false;
	//removeCredits = false;
	
	isScrolling = true;
	position.x  = -2800.f;
	scrollDest	= -1080.f;
	numScrolls	= 0;

	startLVL	= 0;
}

MainMenuLayer::~MainMenuLayer()
{
	if (buttonFont)
	{
		delete buttonFont;
	}

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

	listenFrame();
	listenKeys();
}
void MainMenuLayer::loadButtons()
{
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


	// Load the font
	buttonFont =  new Pim::Font("res/arial.ttf", 50, true);

	//std::string menyButtonLabels[NUMMENYBUTTONS]  = {"Start", "Options", "Highscore", "Exit"};
	std::string menyButtonLabels[NUMMENYBUTTONS]  = {"Start lvl1", "Start lvl2", "Start lvl3", "Credits"};

	// Create the buttons
	for(int i = 0; i < NUMMENYBUTTONS; i ++)
	{
		menuButtons[i]			= createButton(1272, SCREENHEIGHT + 30, menyButtonLabels[i]);
		buttonYPos[i]			= 108 - (PTMR*i*3), menyButtonLabels[i];
	}

	menuButtons[0]->setZOrder(1000);
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

MenuButton* MainMenuLayer::createButton(int xPos, int yPos, std::string buttonLabel)
{

	float scaleX = 1.0f;
	float scaleY = 0.7f;


    Pim::Sprite *normal = new Pim::Sprite;
    normal->useBatchNode(buttonSheet) ; 
    normal->rect = Pim::Rect(0,0,150,35);
	normal->scale = Pim::Vec2(scaleX, scaleY);

    Pim::Sprite *hovered = new Pim::Sprite;
    hovered->useBatchNode(buttonSheet);
    hovered->rect = Pim::Rect(0,34,150,35);
	hovered->scale = Pim::Vec2(scaleX, scaleY);

    Pim::Sprite *pressed = new Pim::Sprite;
    pressed->useBatchNode(buttonSheet); 
    pressed->rect = Pim::Rect(0,69,150,40);
	pressed->scale = Pim::Vec2(scaleX, scaleY);

	MenuButton *menuButton = new MenuButton(normal, hovered, pressed, buttonFont);
	addChild(menuButton);
	menuButton->position = Pim::Vec2(192, 108);
	menuButton->setText(buttonLabel);

	menuButton->setZOrder(105);

    menuButton->setCallback(this);

	menuButton->position = Pim::Vec2(xPos, yPos);


	return menuButton;
}

void MainMenuLayer::buttonPressed(Pim::Button* activeButton)
{

	if(playCredits)
	{
		return;
	}

	if(activeButton == menuButtons[0]) //start
	{
		//Pim::GameControl::getSingleton()->setScene(new GameScene(1));
		playOutro = true;
		startLVL = 1;
	}
	if(activeButton == menuButtons[1]) //Options
	{
		playOutro = true;
		startLVL = 2;
	}
	if(activeButton == menuButtons[2]) //Highscore
	{
		playOutro = true;
		startLVL = 3;
	}
	if(activeButton == menuButtons[3]) //Credits
	{
		playCredits = true;
		startLVL = 3;
	}
	if(activeButton == menuButtons[4]) //Exit
	{
		exit(0);
	}
	
	//Pim::GameControl::getSingleton()->setScene(new GameScene(1));
}

//void MainMenuLayer::killTheCredits()
//{
//
//	playCredits = false;
//	delete creditsObject;
//	std::cout << "Barnet mitt er drept!";
//	for(int i = 0; i < NUMMENYBUTTONS; i ++)
//	{
//		menuButtons[i]->activated = true;
//	}
//
//
//}

void MainMenuLayer::update(float dt)
{
	//if(playCredits)
	//{

	//	if(creditsObject == NULL)
	//	{
	//		creditsObject = new CreditsObject(this);
	//		for(int i = 0; i < NUMMENYBUTTONS; i ++)
	//		{
	//			menuButtons[i]->activated = false;
	//		}
	//	}
	//	

	//	return;
	//	
	//}

	updateButtons(dt);
	updateScroll(dt);
}
void MainMenuLayer::updateButtons(float dt)
{
	if(playIntro)
	{
		int numRight = 0;
		for(int i = 0; i < NUMMENYBUTTONS; i ++)
		{
			if(menuButtons[i]->position.y > buttonYPos[i])
			{
				menuButtons[i]->position.y -= (2.8f * MENYSPEED);
			}
			else
			{
				menuButtons[i]->position.y = buttonYPos[i];
				numRight ++;
			}
		}

		if(numRight == NUMMENYBUTTONS)
		{
			playIntro = false;
		}
	}
	
	if(playOutro)
	{
		for(int i = 0; i < NUMMENYBUTTONS; i ++)
		{
			if(menuButtons[i]->position.x > -50 && menuButtons[i]->position.x < SCREENWIDTH + 50)
			{	//2.8f ganer enten -1 erller 1 bassert på index
				menuButtons[i]->position.x += (3.8f * MENYSPEED) * ( ((i % 2) * 2) -1);
			}
			else
			{ 
				isScrolling = true;
				scrollDest  = -20.f;
			}

		}

	}

	/*
	if(startGame)
	{
		Pim::GameControl::getSingleton()->setScene(new GameScene(startLVL));
	}
	*/
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
				playIntro = true;
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