#include "Troll.h"
#include "TrollAI.h"
#include "HUDLayer.h"


Pim::Font* Troll::floatFont = NULL;

void Troll::createFont()
{
	if (!floatFont)
	{
		floatFont = new Pim::Font("res\\arial.ttf", 40, false);
	}
}
void Troll::destroyFont()
{
	if (floatFont)
	{
		delete floatFont;
	}
}

Troll::Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p)
	: Actor(n, p)
{
	walkSpeed	= NULL;
	ai			= NULL;
	listenFrame();
}

void Troll::takeDamage(int damage)
{
	health -= damage;
	addFloatLabel(damage);
}
void Troll::addFloatLabel(int damage)
{
	std::stringstream ss;
	ss << damage;

	FloatLabel *lab = new FloatLabel(floatFont);
	lab->setText(ss.str());
	lab->scale = Pim::Vec2(0.1f, 0.1f);
	lab->color = Pim::Color(1.f, 1.f, 0.f, 1.f);
	lab->position = position + getParentLayer()->position + Pim::Vec2(0.f, 15.f);
	lab->initialX = position.x;
	
	HUDLayer::getSingleton()->addFloatLabel(lab);
}
