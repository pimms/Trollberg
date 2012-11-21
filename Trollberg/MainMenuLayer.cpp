#pragma once 
#include "MainMenuLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"


MainMenuLayer::MainMenuLayer()
{
	mainButton	= NULL;
	buttonSheet	= NULL;
}

void MainMenuLayer::loadResources()
{
	buttonSheet = new Pim::SpriteBatchNode("res/buttonsMainMenu.png");
    addChild(buttonSheet);

	for(int i = 0; i < 4; i ++)
	{
		mainButton = createButton(192, 108 - (PTMR*i*3));
	}
}

Pim::Button* MainMenuLayer::createButton(int xPos, int yPos)
{
    Pim::Sprite *normal = new Pim::Sprite;
    normal->useBatchNode(buttonSheet); 
    normal->rect = Pim::Rect(0,0,40,25);

    Pim::Sprite *hovered = new Pim::Sprite;
    hovered->useBatchNode(buttonSheet);
    hovered->rect = Pim::Rect(40,0,40,25);

    Pim::Sprite *pressed = new Pim::Sprite;
    pressed->useBatchNode(buttonSheet); 
    pressed->rect = Pim::Rect(80,0,40,25);

    Pim::Button *button = new Pim::Button(normal, hovered, pressed, NULL);
    button->setCallback(this);
    addChild(button);

	//button->position = Pim::Vec2(192, 108);
	//button->position = Pim::Vec2(rand()%384, rand()%216);
	button->position = Pim::Vec2(xPos, yPos);


	return button;
}

void MainMenuLayer::buttonPressed(Pim::Button*)
{
	Pim::GameControl::getSingleton()->setScene(new GameScene(1));
}
