#pragma once 
#include "Pim.h"

class MainMenuLayer: public Pim::Layer, public Pim::ButtonCallback
{
public:
	MainMenuLayer();

	void loadResources();

	Pim::Button* createButton(int xPos, int yPos);

	void buttonPressed(Pim::Button*);

private:
	Pim::SpriteBatchNode *buttonSheet;

	Pim::Button *mainButton;
};