#include "LightRifle.h"
#include "GameScene.h"

LightRifle::LightRifle(Pim::SpriteBatchNode *b)
	: Weapon(b, Pim::Rect(0,14,14,5))
{
	minDamage	= 20;
	maxDamage	= 30;
	accuracy	= 5.f;
	rof			= 0.1f;
	rofAlt		= 1.f;

	position = Pim::Vec2(3.f, -2.f);
	anchor = Pim::Vec2(0.2f, 0.5f);
}

void LightRifle::fire()
{
	if (timer >= rof)
	{
		LRBullet *b = new LRBullet(actorSheet, getLayerPosition(), angle());
		b->listenFrame();
		getParentLayer()->addChild(b);
		bullets.push_back(b);

		Pim::SmoothLightDef *ld = new Pim::SmoothLightDef;
		//ld->radius = 200.f;
		ld->radius = 150.f+( rand() % 150);
		ld->innerColor.a = 0.7f;
		getParentLayer()->addLight(b, ld);

		timer = 0.f;
	}
}

void LightRifle::altFire()
{
	if (timerAlt >= rofAlt)
	{
		LRBullet *b = new LRBullet(actorSheet, getLayerPosition(), angle());
		b->listenFrame();
		getParentLayer()->addChild(b);
		bullets.push_back(b);

		Pim::FlatLightDef *ld = new Pim::FlatLightDef;
		ld->radius = 600.f;
		ld->falloff = 0.1f;
		ld->innerColor.a = 0.7f;
		getParentLayer()->addLight(b, ld);

		timerAlt = 0.f;
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


// -----------------------------------


LRBullet::LRBullet(Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle)
{
	_tw = 3;
	_th = 3;
	rect		= Pim::Rect(0,0,3,3);
	position	= pos;
	life		= 0.f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.allowSleep = false;
	bd.position = toB2(pos);
	bd.bullet = true;

	b2CircleShape shape;
	shape.m_radius = 1.f/PTMR;

	b2FixtureDef fd;
	fd.restitution = 1.f;
	fd.shape = &shape;
	fd.userData = this;
	fd.filter.categoryBits = SENSOR;
	fd.filter.maskBits = GROUND;

	body = GameScene::getWorld()->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	vel = b2Vec2( cosf(angle*DEGTORAD)*37, sinf(angle*DEGTORAD)*37);
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
				deleteBody();
				parent->removeChild(this,true);
				return;
			}
		}

		body->SetLinearVelocity(vel);
	}
}