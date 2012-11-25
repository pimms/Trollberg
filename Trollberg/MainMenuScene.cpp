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
	color = Pim::Color(0.207, 0.537, 0.647, 1.f);

	addLayer(new MainMenuLayer());
	loadMainMenu();

	// Maximize the window
	//HWND hwnd = Pim::GameControl::getSingleton()->getRenderWindow()->getHwnd();
	//SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void MainMenuScene::startGame()
{
	Pim::GameControl::getSingleton()->setScene(new GameScene(1));
}





