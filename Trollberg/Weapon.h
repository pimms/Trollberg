#pragma once

#include "pimsprite.h"
#include "Entity.h"

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
	Weapon(Pim::SpriteBatchNode *batch, Pim::Rect r);
	Weapon(const Weapon&) {}

	void update(float dt);

	Pim::SpriteBatchNode 	*actorSheet;
	int 	minDamage;	// The minimum damage
	int		maxDamage;	// The maximum damage
	float 	accuracy;	// +/-  degrees 
	float 	rof;		// Rate of fire - bullets per second

	float	timer;		// Time since last bullet was fired
};


// ----------- LIGHT RIFLE ----------- //
class LRBullet;
class LightRifle : public Weapon
{
public:
	LightRifle(Pim::SpriteBatchNode *batch);

	void fire();
	void setMirrored(bool flag);

	void update(float dt);

private:
	std::list<LRBullet*> bullets;
};
class LRBullet : public Entity
{
public:
	LRBullet(Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle);
	float life;
};