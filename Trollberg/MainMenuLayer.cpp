#pragma once 
#include "MenuButton.h"
#include "MainMenuLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Pim.h"


MainMenuLayer::MainMenuLayer()
{
	mainButton	= NULL;
	buttonSheet	= NULL;
}

void MainMenuLayer::loadResources()
{
	buttonSheet = new Pim::SpriteBatchNode("res/buttonsMainMenu.png");
    addChild(buttonSheet);

	std::string menyButtonLabels[NUMMENYBUTTONS]  = {"Start", "Options", "Highscore", "Exit"};

	for(int i = 0; i < NUMMENYBUTTONS; i ++)
	{
		std::cout << "i: " << i << " verdi: " << menyButtonLabels[i];
		menuButtonsArray[i] = createButton(192, 108 - (PTMR*i*3), menyButtonLabels[i]);
	}

	menuButtonsArray[0]->setZOrder(0);

	listenFrame();

}

MenuButton* MainMenuLayer::createButton(int xPos, int yPos, std::string buttonLabel)
{
    Pim::Sprite *normal = new Pim::Sprite;
    normal->useBatchNode(buttonSheet) ; 
    normal->rect = Pim::Rect(0,0,40,25);

    Pim::Sprite *hovered = new Pim::Sprite;
    hovered->useBatchNode(buttonSheet);
    hovered->rect = Pim::Rect(40,0,40,25);

    Pim::Sprite *pressed = new Pim::Sprite;
    pressed->useBatchNode(buttonSheet); 
    pressed->rect = Pim::Rect(80,0,40,25);

	Pim::Font *fonten =  new Pim::Font("res/arial.ttf", 50, true);

	MenuButton *menuButton = new MenuButton(normal, hovered, pressed, fonten);
	addChild(menuButton);
	menuButton->position = Pim::Vec2(192, 108);
	menuButton->setText(buttonLabel);

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
		Pim::GameControl::getSingleton()->setScene(new GameScene(1));
	}
	if(activeButton == menuButtonsArray[3]) //Options
	{
		exit(0);
	}
	if(activeButton == menuButtonsArray[3]) //Highscore
	{
		exit(0);
	}
	if(activeButton == menuButtonsArray[3]) //Exit
	{
		exit(0);
	}
	
	//Pim::GameControl::getSingleton()->setScene(new GameScene(1));
}

void MainMenuLayer::update(float dt)
{

	//menuButtonsArray[0]->position = Pim::Vec2(rand()%SCREENWIDTH, rand()%SCREENHEIGHT);

}
