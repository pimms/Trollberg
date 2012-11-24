#pragma once
#include "troll.h"

// Forward declarations
class Player;
class TumblerAI;

class Tumbler :public Troll
{
public:
	Tumbler(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);
	~Tumbler(void);

	void createPhysics();

	void update(float dt);

	void throwRock();

	void takeDamage(int damage);

	void deleteBody();

private:
	friend class TumblerAI;

	// The rectangular shape of the Tumbler and the getting-stuck-oriented nature of
	// box shapes in Box2D requires the Tumbler to use two circular bodies jointed
	// together.
	b2Body		*body2;		

	Animation	walkAnim;
	Animation	throwAnim;
	Animation	deathAnim;

	Player		*player;

	float		deathTimer;
	bool		dead;
};