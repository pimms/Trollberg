#include "Revolver.h"
#include "GameScene.h"
#include "GameLayer.h"
#include "Bullet.h"
#include "Troll.h"


Revolver::Revolver(Pim::SpriteBatchNode *b)
	: Weapon(b, Pim::Rect(150,0,10,5))
{
	minDamage	= 15;
	maxDamage	= 25;
	accuracy	= 5.f;
	rof			= 0.f;
	rofAlt		= 1.f;

	position	= Pim::Vec2(-2.f, 1.f);
	anchor		= Pim::Vec2(0.f, 0.4f);
}

void Revolver::removeChild(Pim::GameNode *node, bool cleanup)
{
	Pim::GameNode::removeChild(node, cleanup);
}

void Revolver::fire()
{
	if (timer >= rof)
	{
		Pim::Sound *s = new Pim::Sound;
		s->useCache("pis");
		s->deleteWhenDone = true;
		s->play();

		GameLayer::getSingleton()->vibrate(1.f, 1.f, 0.1f);

		Bullet *bullet = new Bullet(this, actorSheet, muzzlePoint(), angle());
		getParentLayer()->addChild(bullet);

		// The bullet must create it's light AFTER it's been parented.
		bullet->createLight();
		bullet->scale *= 0.5f;

		timer = 0.f;
	}
}
void Revolver::altFire()
{
	if (timerAlt >= rofAlt)
	{
		Bullet *b = new Bullet(this, actorSheet, muzzlePoint(), angle());
		b->listenFrame();
		getParentLayer()->addChild(b);

		Pim::FlatLightDef *ld = new Pim::FlatLightDef;
		ld->radius = 600.f;
		ld->falloff = 0.1f;
		ld->innerColor.a = 0.7f;
		getParentLayer()->addLight(b, ld);

		timerAlt = 0.f;
	}
}
void Revolver::setMirrored(bool flag)
{
	if (flag)
	{
		position.x = 2.f;
		scale.y = -1.f;
	}
	else
	{
		position.x = -2.f;
		scale.y = 1.f;
	}
}
Pim::Vec2 Revolver::muzzlePoint()
{
	if (scale.y > 0.f)
	{
		float a = rotation * DEGTORAD;
		return getLayerPosition() + Pim::Vec2(8*cosf(a), 8*sinf(a));
	}
	else
	{
		float a = rotation * DEGTORAD;
		return getLayerPosition() + Pim::Vec2(8*cosf(a), 8*sinf(a));
	}
}

void Revolver::update(float dt)
{
	timer += dt;
}
