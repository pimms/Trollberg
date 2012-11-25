#pragma once

#include "pimsprite.h"
#include "Entity.h"

enum WeaponID
{
	REVOLVER,
	SHOTGUN,
	SNIPER,
};

// Forward declarations
class Bullet;

class Weapon : public Pim::Sprite
{
public:
	static Weapon* createWeapon(Pim::SpriteBatchNode *b, WeaponID);

	virtual float damage(Bullet*);		// Returns a calculated damage value
	virtual float angle();		// Returns a calculated fire angle

	virtual void fire() = 0;	// Fire the bullet. 
	virtual void altFire() = 0;	// Fire the bullet. 
	virtual void setMirrored(bool flag) = 0;
	virtual Pim::Vec2 muzzlePoint() = 0;

protected:
	// Private constructors for the sake of fuck you.
	Weapon(Pim::SpriteBatchNode *batch, Pim::Rect r);
	Weapon(const Weapon&) {}

	void update(float dt);

	Pim::SpriteBatchNode 	*actorSheet;
	int 	minDamage;	// The minimum damage
	int		maxDamage;	// The maximum damage
	float 	accuracy;	// +/-  degrees 
	float 	rof;		// Rate of fire - bullets per second
	float 	rofAlt;		// Rate of fire on Alt fire - bullets per second

	float	timer;		// Time since last bullet was fired
	float	timerAlt;		// Time since last bullet was fired in Altfire
};
