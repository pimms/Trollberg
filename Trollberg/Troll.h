#pragma once
#include "actor.h"

// Forward declarations
class TrollAI;
class SlinkerAI;
class HUDLayer;

class FloatLabel : public Pim::Label
{
public:
	FloatLabel(Pim::Font *f) : Pim::Label(f) { lifetime = 0.f; }
	float lifetime;
	float initialX;
	float initialLayerX;
};

class Troll : public Actor
{
public:
	static void createFont();
	static void destroyFont();

	Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p);
	~Troll();

	virtual void update(float dt);
	
	virtual void takeDamage(int damage);
	void addFloatLabel(int damage);

	virtual void deleteBody();

protected:
	friend class TrollAI;

	static Pim::Font *floatFont;

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

