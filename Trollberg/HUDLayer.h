#pragma once
#include "Pim.h"

// Forward declarations
class Troll;
class DamageLabel;

class HUDLayer : public Pim::Layer
{
public:
	static HUDLayer* getSingleton();

	static void createFont();
	static Pim::Font* getFont();
	static void destroyFont();

	HUDLayer();
	~HUDLayer();

	void loadResources();
	void loadHelperSprites();		// Loads the esc-menu and sound indicator
	void loadWeaponCogs();			// Loads the weapon cogs
	void loadPowerCogs();			// Loads the cogs spinning near the power-bar
	void loadPowerBar();			// Loads the power bar and it's background
	void loadHearts();				// Loads the hearts

	void update(float dt);
	void updateWeaponCog(float dt);

	void updateLabels(float dt);
	void addDamageLabel(Troll *t, int damage);

	void setPlayerHealth(int health);
	void setSelectedWeapon(int wep);

private:
	static HUDLayer	*singleton;
	static Pim::Font *arial40;

	Pim::SpriteBatchNode *actorSheet;

	Pim::Label		*FPSLabel;

	Pim::Sprite		*fadeSprite;	// Fade the HUD from black
	bool			isFading;		// Done fading?
	float			fadeTimer;		// Keep track of fading

	int				curWeapon;		// The currently selected weapon
	int				weaponRotDir;	// Rotation direction of the weapon cog
	float			weaponRotDest;	// The angle the weapon cog is rotation towards

	// The hearts
	Pim::Sprite		*hearts[3];

	// The various cogs in the HUD
	Pim::Sprite		*weaponCog;
	Pim::Sprite		*weapons[3];
	Pim::Sprite		*arrowCog;

	Pim::Sprite		*powerCogs[3];

	// The mute / unmute indicator
	Pim::Sprite		*soundInd;

	// Indicated that a troll has been hit
	class DamageLabel : public Pim::Label
	{
	public:
		DamageLabel(Pim::Font *f) : Pim::Label(f) { lifetime = 0.f; }
		float lifetime;
		float initialX;
		float initialLayerX;
	};

	std::vector<DamageLabel*>	labels;
};

