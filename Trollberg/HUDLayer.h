#pragma once
#include "Pim.h"

// Forward declarations
class FloatLabel;	// Declared in Troll.h

class HUDLayer : public Pim::Layer
{
public:
	static HUDLayer* getSingleton();

	HUDLayer();
	~HUDLayer();

	void loadResources();
	void loadHelperSprites();		// Loads the esc-menu and sound indicator
	void loadWeaponCogs();			// Loads the weapon cogs
	void loadPowerCogs();			// Loads the cogs spinning near the power-bar
	void loadPowerBar();			// Loads the power bar and it's background
	void loadHearts();				// Loads the hearts

	void update(float dt);

	void updateLabels(float dt);
	void addFloatLabel(FloatLabel *lab);

	void setPlayerHealth(int health);

private:
	static HUDLayer	*singleton;

	std::vector<FloatLabel*>	labels;

	Pim::SpriteBatchNode *actorSheet;

	Pim::Sprite		*fadeSprite;	// Fade the HUD from black
	bool			isFading;		// Done fading?
	float			fadeTimer;		// Keep track of fading

	Pim::Label		*FPSLabel;

	// The hearts
	Pim::Sprite		*hearts[3];

	// The various cogs in the HUD
	Pim::Sprite		*weaponCog;
	Pim::Sprite		*weapons[3];
	Pim::Sprite		*arrowCog;

	Pim::Sprite		*powerCogs[3];

	// The mute / unmute indicator
	Pim::Sprite		*soundInd;
};

