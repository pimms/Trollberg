#include "Trollberg.h"
#include "LRBullet.h"
#include "LightRifle.h"
#include "GameScene.h"
#include "Troll.h"

LRBullet::LRBullet(LightRifle *lr, Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle)
{
	_tw			= 3;
	_th			= 3;
	rect		= Pim::Rect(0,0,3,3);
	position	= pos;
	life		= 0.f;
	lightRifle	= lr;

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
	lightDef->radius = 40 + rand()%20;
	getParentLayer()->addLight(this, lightDef, "LRBullet");
}

void LRBullet::update(float dt)
{
	life += dt;
	
	if (life >= 1.f)
	{
		deleteBody();
		parent->removeChild(this, true);
	}
	else
	{
		for (auto c=body->GetContactList(); c; c=c->next)
		{
			if (c->contact->IsTouching())
			{
				// Is the bullet touching the ground?
				if (otherCollidingFixture(c->contact, GROUND))
				{
					deleteBody();
					parent->removeChild(this,true);
					return;
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
					}
				}
			}
		}

		body->SetLinearVelocity(vel);
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