#pragma once 
#include "MainMenuScene.h"
#include "GameScene.h"
#include "MainMenuLayer.h"



MainMenuScene::MainMenuScene(){

	//loadMainMenu();
	//1
	//2
}

void MainMenuScene::loadMainMenu(){



	//Sleep(2000);

	//startGame();

}

void MainMenuScene::loadResources(){

	addLayer(new MainMenuLayer());

	//Pim::GameControl::getSingleton()->setScene(new GameScene(1));
	loadMainMenu();

}

void MainMenuScene::startGame(){

	Pim::GameControl::getSingleton()->setScene(new GameScene(1));

}





