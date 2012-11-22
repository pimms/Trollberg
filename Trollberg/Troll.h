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

	virtual void update(float dt) = 0;
	
	virtual void takeDamage(int damage);
	void addFloatLabel(int damage);

protected:
	static Pim::Font *floatFont;

	friend class TrollAI;
	TrollAI						*ai;

	float						walkSpeed;
};

