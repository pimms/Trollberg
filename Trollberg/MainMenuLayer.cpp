#pragma once 
#include "MainMenuLayer.h"
#include "MainMenuScene.h"
#include "GameScene.h"


MainMenuLayer::MainMenuLayer(){



}

void MainMenuLayer::loadResources(){

	//for(int i = 0; i < 500; i ++)
		mainButton = addButton();



}

Pim::Button* MainMenuLayer::addButton(){

	Pim::SpriteBatchNode *buttonSheet = new Pim::SpriteBatchNode("res/buttonsMainMenu.png");
    addChild(buttonSheet);

    Pim::Sprite *normal = new Pim::Sprite;
    normal->useBatchNode(buttonSheet); normal->rect = Pim::Rect(0,0,40,25);

    Pim::Sprite *hovered = new Pim::Sprite;
    hovered->useBatchNode(buttonSheet);
    hovered->rect = Pim::Rect(40,0,40,25);

    Pim::Sprite *pressed = new Pim::Sprite;
    pressed->useBatchNode(buttonSheet); pressed->rect = Pim::Rect(80,0,40,25);

    Pim::Button *button = new Pim::Button(normal, hovered, pressed, NULL);
    button->setCallback(this);
    addChild(button);

	button->position = Pim::Vec2(192, 108);
	//button->position = Pim::Vec2(rand()%385, rand()%216);

	Pim::Font *fonten =  new Pim::Font("res/arial.ttf", 50, true);
	Pim::Label *labelen = new Pim::Label(fonten, "dette er en fucker");
	labelen->color = Pim::Color(1.f, 1.f, 1.f, 1.f);
	labelen->position = Pim::Vec2(112, 108);

	addChild(labelen);

	return button;

}



void MainMenuLayer::buttonPressed(Pim::Button*){

	Pim::GameControl::getSingleton()->setScene(new GameScene(1));

}
void MainMenuLayer::buttonReleased(Pim::Button*){
}
void MainMenuLayer::buttonHoverBegin(Pim::Button* eventButton){


	//eventButton->position = Pim::Vec2(rand()%385, rand()%216);
	//removeChild(eventButton, true);

}
void MainMenuLayer::buttonHoverEnd(Pim::Button*){


}