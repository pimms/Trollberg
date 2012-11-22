#pragma once
#include "actor.h"
#include "Animation.h"
#include "Weapon.h"

class Player : public Actor
{
public:
	Player(Pim::SpriteBatchNode *node, Pim::Vec2 pos);
	~Player();

	void createLight();

	void keyEvent(Pim::KeyEvent &evt);
	void mouseEvent(Pim::MouseEvent &evt);

	void takeDamage(int damage);

	void update(float dt);	// Handle animation, directional facing, etc
	void updateWeapon();	// Rotate according to the position of the mouse

private:
	float velX;

	Animation				walkAnim;
	Animation				deathAnim;

	Weapon					*weapon;
	Pim::MouseEvent			*mEvt;			// Keep a reference to this
};

