#pragma once
#include "pimsprite.h"

enum WeaponID
{
	LIGHT_RIFLE,
};

class Weapon : public Pim::Sprite
{
public:
	static Weapon* createWeapon(Pim::SpriteBatchNode *b, WeaponID);

	virtual float damage();		// Returns a calculated damage value
	virtual float angle();		// Returns a calculated fire angle

	virtual void fire() = 0;	// Fire the bullet. 
	virtual void setMirrored(bool flag) = 0;

protected:
	// Private constructors for the sake of fuck you.
	Weapon(Pim::SpriteBatchNode *batch);
	Weapon(const Weapon&) {}

	Pim::SpriteBatchNode 	*actorSheet;
	int 	minDamage;	// The minimum damage
	int		maxDamage;	// The maximum damage
	float 	accuracy;	// +/-  degrees 
	float 	rof;		// Rate of fire - bullets per second
};


class LightRifle : public Weapon
{
	LightRifle(Pim::SpriteBatchNode *batch);

	void fire();
	void setMirrored(bool flag);
};

