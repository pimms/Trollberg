#include "Shotgun.h"
#include "Bullet.h"
#include "GameLayer.h"

Shotgun::Shotgun(Pim::SpriteBatchNode *batch)
	: Weapon(batch, Pim::Rect(165,0,10,5))
{
	minDamage	= 10;
	maxDamage	= 16;
	accuracy	= 10;
	rof			= 0.7f;
	rofAlt		= -1.f;

	position	= Pim::Vec2(-2.f, 1.f);
	anchor		= Pim::Vec2(0.f, 0.4f);
}

void Shotgun::fire()
{
	if (timer >= rof)
	{
		Pim::Sound *s = new Pim::Sound;
		s->useCache("sg");
		s->deleteWhenDone = true;
		s->play();

		GameLayer::getSingleton()->vibrate(1.f, 1.f, 0.15f);

		for (int i=0; i<10; i++)
		{
			Bullet *bullet = new Bullet(this, actorSheet, muzzlePoint(), angle());
			getParentLayer()->addChild(bullet);

			if (VersionControl::getGLVersion() > LIGHT_GL_VERSION)
			{
				// The bullet must create it's light AFTER it's been parented.
				bullet->createLight();
				bullet->finalLightRadius -= 70;
			}

			bullet->maxLife	= 0.1f;
			bullet->scale *= 0.3f;

			// Slow down some bullets randomly
			float r = (float)rand() / (float)RAND_MAX;
			r /= 3.f;
			bullet->vel *= (0.7f + r);

			timer = 0.f;
		}
	}
}
void Shotgun::altFire()
{

}
void Shotgun::setMirrored(bool flag)
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
Pim::Vec2 Shotgun::muzzlePoint()
{
	float a = rotation * DEGTORAD;
	return getLayerPosition() + Pim::Vec2(13*cosf(a), 13*sinf(a));
}

float Shotgun::damage(Bullet *b)
{
	float dmg = Weapon::damage(b);

	if (b->life >= 0.1f)
	{
		dmg /= (b->life*10);
	}

	return dmg;
}