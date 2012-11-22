#pragma once
#include "PimLayer.h"

// Forward declarations
class FloatLabel;	// Declared in Troll.h

class HUDLayer : public Pim::Layer
{
public:
	static HUDLayer* getSingleton();

	HUDLayer();
	~HUDLayer();

	void loadResources();

	void update(float dt);

	void updateLabels(float dt);
	void addFloatLabel(FloatLabel *lab);

	void setPlayerHealth(int health);

private:
	static HUDLayer	*singleton;

	Pim::Sprite		*hearts[3];

	std::vector<FloatLabel*>	labels;
};

