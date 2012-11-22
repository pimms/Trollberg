#include "Trollberg.h"
#include "LRBullet.h"
#include "LightRifle.h"
#include "GameScene.h"
#include "Troll.h"

// The maximum lifetime of a bullet
#define LBR_MAXLIFE 2.f
#define LBR_FADEINTIME 0.2f
#define LBR_FADEOUTTIME 0.1f

LRBullet::LRBullet(LightRifle *lr, Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle)
{
	position		= pos;
	life			= 0.f;
	lightRifle		= lr;
	dead			= false;
	fadeOutTimer	= LBR_FADEOUTTIME;
	rect			= Pim::Rect(136,0,5,5);
	useBatchNode(actorSheet);

	b2BodyDef bd;
	bd.type			= b2_dynamicBody;
	bd.allowSleep	= false;
	bd.position		= toB2(pos);
	bd.bullet		= true;

	b2CircleShape shape;
	shape.m_radius	= 1.f/PTMR;

	b2FixtureDef fd;
	fd.restitution	= 1.f;
	fd.shape		= &shape;
	fd.userData		= this;
	fd.isSensor		= true;
	fd.filter.maskBits = GROUND | TROLLS;
	fd.filter.categoryBits = SENSOR;

	body = GameScene::getWorld()->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	vel = b2Vec2( cosf(angle*DEGTORAD)*37, sinf(angle*DEGTORAD)*37);
}
void LRBullet::createLight()
{
	lightDef = new Pim::PreloadLightDef;
	lightDef->radius = 1;
	finalLightRadius = 100 + rand()%100;
	getParentLayer()->addLight(this, lightDef, "LRBullet");
}

void LRBullet::update(float dt)
{
	life += dt;

	if (life <= LBR_FADEINTIME)
	{
		lightDef->radius = finalLightRadius * (life/LBR_FADEINTIME);
	}
	else
	{
		// In the range 0.8-1.0
		float factor =  (sinf(life*10.f) + 1.f) / 10.f + 0.8f;
		lightDef->radius = finalLightRadius * factor;
	}
	
	if ((life >= LBR_MAXLIFE && !dead) || (fadeOutTimer <= 0.f))
	{
		deleteBody();
		parent->removeChild(this, true);
	}
	else if (!dead)
	{
		body->SetLinearVelocity(vel);

		for (auto c=body->GetContactList(); c; c=c->next)
		{
			// Is the bullet touching the ground?
			if (otherCollidingFixture(c->contact, GROUND))
			{
				// The bullet is dead and will fade out
				dead	= true;
			}

			// Is the bullet touching a troll?
			b2Fixture *trollFix = NULL;
			trollFix = otherCollidingFixture(c->contact, TROLLS);

			if (trollFix)
			{
				Troll *t = (Troll*)trollFix->GetUserData();
				if (t && !hasDamaged(t))
				{
					damageTroll(t);
					return;
				}
			}
		} 
	}
	else
	{
		fadeOutTimer -= dt;

		// In the range 1.0 - 0.0
		float factor = (fadeOutTimer/LBR_FADEOUTTIME);

		lightDef->radius = finalLightRadius * factor;
		scale = Pim::Vec2(1.f, 1.f) * factor;

		body->SetLinearVelocity(b2Vec2_zero);
	}
}

bool LRBullet::hasDamaged(Troll *troll)
{
	for each (Troll *t in damagedTrolls)
	{
		if (t == troll)
		{
			return true;
		}
	}
	return false;
}
void LRBullet::damageTroll(Troll *t)
{
	t->takeDamage(lightRifle->damage());
	damagedTrolls.push_back(t);
}