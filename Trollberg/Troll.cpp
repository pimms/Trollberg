#include "Troll.h"
#include "TrollAI.h"
#include "HUDLayer.h"

Pim::Font* Troll::floatFont = NULL;
HUDLayer* Troll::hudLayer = NULL;

void Troll::setHUDLayer(HUDLayer *hud)
{
	hudLayer = hud;
}
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
		delete floatFont;
}

Troll::Troll(Pim::SpriteBatchNode *n, Pim::Vec2 p)
	: Actor(n, p)
{
	walkSpeed	= NULL;
	ai			= NULL;
	listenFrame();
}

void Troll::takeDamage(float damage)
{
	health -= damage;
	addFloatLabel(damage);
}
void Troll::addFloatLabel(float dmg)
{
	std::stringstream ss;
	ss << (int)dmg;

	FloatLabel *lab = new FloatLabel(floatFont);
	lab->setText(ss.str());
	//lab->scale = Pim::Vec2(0.1f, 0.1f);
	lab->color = Pim::Color(1.f, 1.f, 0.f, 1.f);
	lab->position = position + getParentLayer()->position + Pim::Vec2(0.f, 15.f);
	
	hudLayer->addChild(lab);
	labels.push_back(lab);
}
void Troll::updateFloatLabels(float dt)
{
	for (unsigned int i=0; i<labels.size(); i++)
	{
		labels[i]->lifetime		+= dt;
		labels[i]->color.a		-= dt / 1.f;

		labels[i]->position		= position + getParentLayer()->position;
		labels[i]->position.y	+= labels[i]->lifetime * 40.f + 15.f;

		if (labels[i]->lifetime >= 1.f)
		{
			hudLayer->removeChild(labels[i], true);
			labels.erase(labels.begin() + i);
		}
	}
}