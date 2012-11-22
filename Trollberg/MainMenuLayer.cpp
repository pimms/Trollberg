#pragma once 
#include "MenuButton.h"
#include "MainMenuLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Pim.h"


MainMenuLayer::MainMenuLayer()
{
	buttonSheet	= NULL;

	playIntro = false;
	playOutro = false;
	startGame = false;

	startLVL = 0;
}

MainMenuLayer::~MainMenuLayer()
{
	if (buttonFont)
	{
		delete buttonFont;
	}
}

void MainMenuLayer::loadResources()
{
	buttonSheet = new Pim::SpriteBatchNode("res/buttonsMainMenu.png");
    addChild(buttonSheet);
	
	buttonFont =  new Pim::Font("res/arial.ttf", 50, true);

	//std::string menyButtonLabels[NUMMENYBUTTONS]  = {"Start", "Options", "Highscore", "Exit"};
	std::string menyButtonLabels[NUMMENYBUTTONS]  = {"Start lvl1", "Start lvl2", "Start lvl3", "Exit"};

	for(int i = 0; i < NUMMENYBUTTONS; i ++)
	{
		std::cout << "i: " << i << " verdi: " << menyButtonLabels[i];
		menuButtonsArray[i] = createButton(192, SCREENHEIGHT + 30, menyButtonLabels[i]);
		menuButtonsAndPosYArray[i] = 108 - (PTMR*i*3), menyButtonLabels[i];
	}

	menuButtonsArray[0]->setZOrder(1000);

	listenFrame();

}

MenuButton* MainMenuLayer::createButton(int xPos, int yPos, std::string buttonLabel)
{
    Pim::Sprite *normal = new Pim::Sprite;
    normal->useBatchNode(buttonSheet) ; 
    normal->rect = Pim::Rect(0,0,40,25);
	normal->scale = Pim::Vec2(2.5f, 1.f);

    Pim::Sprite *hovered = new Pim::Sprite;
    hovered->useBatchNode(buttonSheet);
    hovered->rect = Pim::Rect(40,0,40,25);
	hovered->scale = Pim::Vec2(2.5f, 1.f);

    Pim::Sprite *pressed = new Pim::Sprite;
    pressed->useBatchNode(buttonSheet); 
    pressed->rect = Pim::Rect(80,0,40,25);
	pressed->scale = Pim::Vec2(2.5f, 1.f);

	MenuButton *menuButton = new MenuButton(normal, hovered, pressed, buttonFont);
	addChild(menuButton);
	menuButton->position = Pim::Vec2(192, 108);
	menuButton->setText(buttonLabel);

	menuButton->setZOrder(105);

    menuButton->setCallback(this);

	//button->position = Pim::Vec2(192, 108);
	//button->position = Pim::Vec2(rand()%384, rand()%216);
	menuButton->position = Pim::Vec2(xPos, yPos);


	return menuButton;
}

void MainMenuLayer::buttonPressed(Pim::Button* activeButton)
{

	if(activeButton == menuButtonsArray[0]) //start
	{
		//Pim::GameControl::getSingleton()->setScene(new GameScene(1));
		playOutro = true;
		startLVL = 1;
	}
	if(activeButton == menuButtonsArray[1]) //Options
	{
		playOutro = true;
		startLVL = 2;
	}
	if(activeButton == menuButtonsArray[2]) //Highscore
	{
		playOutro = true;
		startLVL = 3;
	}
	if(activeButton == menuButtonsArray[3]) //Exit
	{
		exit(0);
	}
	
	//Pim::GameControl::getSingleton()->setScene(new GameScene(1));
}

void MainMenuLayer::update(float dt)
{
	updateButtons(dt);
}
void MainMenuLayer::updateButtons(float dt)
{
	if(playIntro)
	{
		int numRight = 0;
		for(int i = 0; i < NUMMENYBUTTONS; i ++)
		{
			if(menuButtonsArray[i]->position.y > menuButtonsAndPosYArray[i])
			{
				menuButtonsArray[i]->position.y -= (2.8f * MENYSPEED);
			}else
			{
				menuButtonsArray[i]->position.y = menuButtonsAndPosYArray[i];
				numRight ++;
			}
		}

		if(numRight == NUMMENYBUTTONS)
		{
			std::cout << "JAJAJAJA";
			playIntro = false;
		}

	}
	
	if(playOutro)
	{
		for(int i = 0; i < NUMMENYBUTTONS; i ++)
		{

			if(menuButtonsArray[i]->position.x > 0 - 50 && menuButtonsArray[i]->position.x < SCREENWIDTH + 50)
			{	//2.8f ganer enten -1 erller 1 bassert på index
				menuButtonsArray[i]->position.x += (3.8f * MENYSPEED) * ( ((i % 2) * 2) -1);
			}else
			{ //start game when buttons are offscreen
				startGame = true;
			}

		}

	}

	if(startGame)
	{
		Pim::GameControl::getSingleton()->setScene(new GameScene(startLVL));
	}
}