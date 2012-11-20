#pragma once 
#include "Pim.h"

class MainMenuLayer: public Pim::Layer, public Pim::ButtonCallback{

private:

	Pim::Button *mainButton;

public:
	MainMenuLayer();

	void loadResources();

	Pim::Button* addButton();

	void buttonPressed(Pim::Button*);
	void buttonReleased(Pim::Button*);
	void buttonHoverBegin(Pim::Button*);
	void buttonHoverEnd(Pim::Button*); 



};