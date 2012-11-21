#pragma once 
#include "MainMenuLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"


MainMenuLayer::MainMenuLayer()
{
	mainButton = NULL;
}

void MainMenuLayer::loadResources()
{
	mainButton = addButton();
}

Pim::Button* MainMenuLayer::addButton(){

	Pim::SpriteBatchNode *buttonSheet = new Pim::SpriteBatchNode("res/buttonsMainMenu.png");
    addChild(buttonSheet);

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

	button->position = Pim::Vec2(192, 108);

	return button;
}



void MainMenuLayer::buttonPressed(Pim::Button*)
{
	Pim::GameControl::getSingleton()->setScene(new GameScene(1));
}
void MainMenuLayer::buttonReleased(Pim::Button*)
{
}
void MainMenuLayer::buttonHoverBegin(Pim::Button* eventButton)
{
}
void MainMenuLayer::buttonHoverEnd(Pim::Button*)
{
}