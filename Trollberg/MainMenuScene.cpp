#pragma once 
#include "MainMenuScene.h"
#include "GameScene.h"
#include "MainMenuLayer.h"



MainMenuScene::MainMenuScene()
{
}

void MainMenuScene::loadMainMenu()
{
}

void MainMenuScene::loadResources()
{
	addLayer(new MainMenuLayer());

	loadMainMenu();
}

void MainMenuScene::startGame()
{
	Pim::GameControl::getSingleton()->setScene(new GameScene(1));
}





