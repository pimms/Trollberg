#include "LightRifle.h"
#include "GameScene.h"
#include "LRBullet.h"
#include "Troll.h"

LightRifle::LightRifle(Pim::SpriteBatchNode *b)
	: Weapon(b, Pim::Rect(0,14,14,5))
{
	minDamage	= 20;
	maxDamage	= 30;
	accuracy	= 5.f;
	rof			= 0.1f;

	position = Pim::Vec2(3.f, -2.f);
	anchor = Pim::Vec2(0.2f, 0.5f);
}

void LightRifle::removeChild(Pim::GameNode *node, bool cleanup)
{
	for (auto it=bullets.begin(); it!=bullets.end(); it++)
	{
		if ((*it) == node)
		{
			bullets.erase(it);
		}
	}

	Pim::GameNode::removeChild(node, cleanup);
}

void LightRifle::fire()
{
	if (timer >= rof)
	{
		LRBullet *b = new LRBullet(this, actorSheet, getLayerPosition(), angle());
		b->listenFrame();
		getParentLayer()->addChild(b);
		bullets.push_back(b);

		Pim::SmoothLightDef *ld = new Pim::SmoothLightDef;
		ld->radius = 300.f;
		ld->innerColor.a = 0.7f;
		getParentLayer()->addLight(b, ld);

		timer = 0.f;
	}
}
void LightRifle::setMirrored(bool flag)
{
	if (flag)
	{
		position.x = -3.f;
		scale.y = -1.f;
	}
	else
	{
		position.x = 3.f;
		scale.y = 1.f;
	}
}
void LightRifle::update(float dt)
{
	Weapon::update(dt);
}
