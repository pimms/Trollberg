#pragma once
#include "actor.h"
#include "Animation.h"

class Player : public Actor
{
public:
	Player(Pim::SpriteBatchNode *node, Pim::Vec2 pos);
	~Player();

	void createLight();

	void keyEvent(Pim::KeyEvent &evt);
	void update(float dt);

private:
	float velX;

	Animation	anim;
};

