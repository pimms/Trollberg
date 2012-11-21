#pragma once
#include "actor.h"

// Forward declarations
class TrollAI;
class HUDLayer;

class FloatLabel : public Pim::Label
{
public:
	FloatLabel(Pim::Font *f) : Pim::Label(f) { lifetime = 0.f; }
	float lifetime;
};

class Troll : public Actor
{
public:
	static void setHUDLayer(HUDLayer *hud);
	static void createFont();
	static void destroyFont();

	Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p);

	virtual void update(float dt) = 0;
	
	virtual void takeDamage(int damage);
	void addFloatLabel(int damage);
	void updateFloatLabels(float dt);

protected:
	static Pim::Font *floatFont;
	static HUDLayer *hudLayer;

	friend class TrollAI;
	TrollAI		*ai;

	float		walkSpeed;

	std::vector<FloatLabel*> labels;
};

