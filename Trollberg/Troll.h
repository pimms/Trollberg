#pragma once
#include "actor.h"

// Forward declarations
class TrollAI;
class SlinkerAI;
class HUDLayer;

class Troll : public Actor
{
public:
	Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p);
	~Troll();

	virtual void update(float dt);
	
	virtual void takeDamage(int damage);
	void addDamageLabel(int damage);

	virtual void deleteBody();

protected:
	friend class TrollAI;

	TrollAI						*ai;

	b2Body						*body2;

	Animation					walkAnim;
	Animation					attackAnim;
	Animation					deathAnim;

	float						walkSpeed;

	float						timeToDie;
	float						fadeTime;

	float						deathTimer;
	bool						dead;
	bool						isFading;
};

