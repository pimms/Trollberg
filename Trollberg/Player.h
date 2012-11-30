#pragma once
#include "actor.h"
#include "Animation.h"
#include "Weapon.h"

// Forward declarations

class Player : public Actor
{
public:
	Player(Pim::SpriteBatchNode *node, Pim::Vec2 pos);
	~Player();

	void createLight();

	void keyEvent(Pim::KeyEvent &evt);
	void mouseEvent(Pim::MouseEvent &evt);
	void controllerEvent(Pim::ControllerEvent &evt);

	void takeDamage(int damage);

	void update(float dt);	// Handle animation, directional facing, etc
	void updateWeapon();	// Rotate according to the position of the mouse

	// In the range 0-2
	void setActiveWeapon(int wep); 

private:
	friend class GameLayer;

	float velX;

	float healthTimer;

	Pim::LightDef			*lightDef;

	Animation				walkAnim;
	Animation				deathAnim;

	Pim::Sprite				*jpFire;		// the jetpack fire-sprite
	float					jpFuel;			// The fuel in the jetpack
	bool					usingJp;		// Using JP right now?
	bool					reqJpRelease;	// If the player ran out of fuel

	// Keep all weapons instantiated at all times
	Weapon					*allWeapons[3];	
	
	// The currently active weapon
	int						curWep;
	Weapon					*weapon;

	// We're handling the aiming in 
	Pim::MouseEvent			*mEvt;

	// Was the last received walk-input keyboard originated?
	bool					keyMovement;

	// Force the player to single-fire using the controller
	bool					RTReleaseRequired;

	// Are we dead?
	bool					dead;
	float					deathTimer;
};

