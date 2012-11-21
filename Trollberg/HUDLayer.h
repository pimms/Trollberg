#pragma once
#include "PimLayer.h"

class HUDLayer : public Pim::Layer
{
public:
	static HUDLayer* getSingleton();

	HUDLayer();
	~HUDLayer();

	void loadResources();

	void setPlayerHealth(int health);

private:
	static HUDLayer	*singleton;

	Pim::Sprite		*hearts[3];
};

