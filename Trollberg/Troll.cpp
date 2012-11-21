#include "Troll.h"
#include "TrollAI.h"
#include "HUDLayer.h"


#define FL_LIFETIME 0.5f


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
	
	hudLayer->addChild(lab);
	labels.push_back(lab);
}
void Troll::updateFloatLabels(float dt)
{
	for (unsigned int i=0; i<labels.size(); i++)
	{
		labels[i]->lifetime		+= dt;
		labels[i]->color.a		-= dt / FL_LIFETIME;

		labels[i]->scale = Pim::Vec2(1.f, 1.f) * floatLabelScale(labels[i]);

		//labels[i]->position		= position + getParentLayer()->position;
		labels[i]->position.x	 = labels[i]->initialX + getParentLayer()->position.x;
		labels[i]->position.y	+= dt * labels[i]->lifetime * 40.f;

		if (labels[i]->lifetime >= FL_LIFETIME)
		{
			hudLayer->removeChild(labels[i], true);
			labels.erase(labels.begin() + i);
		}
	}
}
float Troll::floatLabelScale(FloatLabel *l)
{
	// f(x) = 2*pow((x-0.1),3) + 0.2*sin((x+0.1)*6)
	// http://www.abhortsoft.hu/functionvisualizer/functionvisualizer.html

	float x = l->lifetime;
	return 2.f*pow((x-0.1f),3.f) + 0.2f*sinf((x+0.1f)*6.f);
}