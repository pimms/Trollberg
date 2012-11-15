#pragma once
#include "pimsprite.h"

enum WeaponID
{
	LIGHT_RIFLE,
};

class Weapon : public Pim::Sprite
{
public:
	static Weapon* createWeapon(WeaponID);

protected:
	Weapon() {}
	Weapon(const Weapon&) {}
};


class LightRifle : public Weapon
{

};

