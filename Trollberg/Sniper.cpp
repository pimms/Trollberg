#include "Sniper.h"
#include "Bullet.h"
#include "GameLayer.h"

Sniper::Sniper(Pim::SpriteBatchNode *batch)
	: Weapon(batch, Pim::Rect(180,0,15,6))
{
	minDamage	= 50;
	maxDamage	= 150;
	accuracy	= 0.f;
	rof			= 1.f;
	rofAlt		= -1.f;

	position	= Pim::Vec2(-2.f, 1.f);
	anchor		= Pim::Vec2(0.f, 0.5f);
}

void Sniper::fire()
{
	if (timer >= rof)
	{
		Pim::Sound *s = new Pim::Sound;
		s->useCache("snp");
		s->deleteWhenDone = true;
		s->play();

		GameLayer::getSingleton()->vibrate(1.f, 1.f, 0.4f);

		Bullet *bullet = new Bullet(this, actorSheet, muzzlePoint(), angle());
		getParentLayer()->addChild(bullet);

		// The bullet must create it's light AFTER it's been parented.
		bullet->createLight();
		bullet->rect = Pim::Rect(200,0,13,3);
		
		bullet->finalLightRadius += 100;

		timer = 0.f;
	}
}
void Sniper::altFire()
{

}
void Sniper::setMirrored(bool flag)
{
	if (flag)
	{
		scale.y = -1.f;
		position.x = 2.f;
	}
	else
	{
		scale.y = 1.f;
		position.x = -2.f;
	}
}
Pim::Vec2 Sniper::muzzlePoint()
{
	float a = rotation*DEGTORAD;
	return getLayerPosition() + Pim::Vec2(15*cosf(a), 15*sinf(a));
}

float Sniper::damage(Bullet *b)
{
	return Weapon::damage(b) - 50.f * b->life;
}