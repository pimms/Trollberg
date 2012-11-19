#include "Weapon.h"
#include "GameScene.h"

Weapon* Weapon::createWeapon(Pim::SpriteBatchNode *b, WeaponID wid)
{
	switch (wid)
	{
		case LIGHT_RIFLE:
			return new LightRifle(b);
	}

	return NULL;
}

Weapon::Weapon(Pim::SpriteBatchNode *b, Pim::Rect r)
	: Pim::Sprite()
{
	listenFrame();

	actorSheet	= b;
	rect		= r;	
	timer		= 10.f;			// Initial high value, instant firing is allowed
	useBatchNode(actorSheet);
}

float Weapon::damage()
{
	// Get a random number between 0 and 1
	float r = (float)rand()/(float)RAND_MAX;
	return minDamage + r*(maxDamage-minDamage);
}
float Weapon::angle()
{
	// Get a random number between -1 and 1
	float r = (float)rand()/((float)RAND_MAX/2.f) - 1.f;
	return rotation + r*accuracy;
}

void Weapon::update(float dt)
{
	timer += dt;
}


// ----------- LIGHT RIFLE ----------- //
LightRifle::LightRifle(Pim::SpriteBatchNode *b)
	: Weapon(b, Pim::Rect(0,14,14,8))
{
	minDamage	= 20;
	maxDamage	= 30;
	accuracy	= 5.f;
	rof			= 0.1f;

	position = Pim::Vec2(3.f, -2.f);
	anchor = Pim::Vec2(0.2f, 0.5f);
}

void LightRifle::fire()
{
	if (timer >= rof)
	{
		LRBullet *b = new LRBullet(actorSheet, getLayerPosition(), angle());
		getParentLayer()->addChild(b);
		bullets.push_back(b);

		Pim::SmoothLightDef *ld = new Pim::SmoothLightDef;
		ld->radius = 15.f;
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

	std::vector<std::list<LRBullet*>::iterator> deleteIndexes;

	for (auto it=bullets.begin(); it!=bullets.end(); it++)
	{
		(*it)->life += dt;
		if ((*it)->life >= 0.5f)
		{
			(*it)->deleteBody();
			(*it)->getParent()->removeChild((*it), true);
			deleteIndexes.push_back(it);
		}
	}
	for each (auto i in deleteIndexes)
		bullets.erase(i);
}
LRBullet::LRBullet(Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle)
{
	//useBatchNode(actorSheet);
	//rect		= Pim::Rect(5,3,2,2);
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

	b2Vec2 force( cosf(angle*DEGTORAD)*4000, sinf(angle*DEGTORAD)*4000);
	body->ApplyForce(force, toB2(position));
}