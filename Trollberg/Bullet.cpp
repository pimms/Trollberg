#include "Trollberg.h"
#include "Bullet.h"
#include "Weapon.h"
#include "GameScene.h"
#include "Troll.h"

// The maximum lifetime of a bullet
#define LBR_MAXLIFE 2.f
#define LBR_FADEINTIME 0.2f
#define LBR_FADEOUTTIME 0.1f

Bullet::Bullet(Weapon *w, Pim::SpriteBatchNode *actorSheet, Pim::Vec2 pos, float angle)
{
	ignoreb2Rotation = true;
	rotation		= angle;
	position		= pos;
	life			= 0.f;
	weapon			= w;
	dead			= false;
	fadeOutTimer	= LBR_FADEOUTTIME;
	rect			= Pim::Rect(136,0,5,5);
	useBatchNode(actorSheet);
	setZOrder(5);

	b2BodyDef bd;
	bd.type			= b2_dynamicBody;
	bd.allowSleep	= false;
	bd.position		= toB2(pos);
	bd.bullet		= true;
	bd.fixedRotation = false;

	b2CircleShape shape;
	shape.m_radius	= 0.2f;;

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

	vel		= b2Vec2( cosf(angle*DEGTORAD)*37, sinf(angle*DEGTORAD)*37);

	maxLife = 2.f;		// Default value

	listenFrame();
}
void Bullet::createLight()
{
	lightDef = new Pim::PreloadLightDef;
	lightDef->radius = 1;
	finalLightRadius = 100 + rand()%101;
	getParentLayer()->addLight(this, lightDef, "Bullet");
}

void Bullet::update(float dt)
{
	life += dt;

	if (VersionControl::getGLVersion() > LIGHT_GL_VERSION)
	{
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
		scale = Pim::Vec2(1.f, 1.f) * factor;

		if (VersionControl::getGLVersion() > LIGHT_GL_VERSION)
		{
			lightDef->radius = finalLightRadius * factor;
		}

		body->SetLinearVelocity(b2Vec2_zero);
	}
}

bool Bullet::hasDamaged(Troll *troll)
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
void Bullet::damageTroll(Troll *t)
{
	t->takeDamage(weapon->damage(this));
	damagedTrolls.push_back(t);
}