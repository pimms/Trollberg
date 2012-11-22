#include "Entity.h"
#include "GameScene.h"


Entity::Entity(void)
{
	body = NULL;
	world = GameScene::getWorld();
}

void Entity::setPosition(Pim::Vec2 pos)
{
	position = pos;
	body->SetTransform(toB2(pos), body->GetAngle());
}
void Entity::setPosition(b2Vec2 pos)
{
	position = toPim(pos);
	body->SetTransform(pos, body->GetAngle());
}

void Entity::createRectangularBody(Pim::Vec2 dimensions, int category, int mask, float density)
{
	b2BodyDef bd;
	bd.type					= b2_dynamicBody;
	bd.fixedRotation		= true;
	bd.allowSleep			= false;
	bd.position				= toB2(position);

	b2PolygonShape shape;
	shape.SetAsBox(dimensions.x/PTMR/2.f, dimensions.y/PTMR/2.f);

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.restitution			= 0.f;
	fd.friction				= 0.f;
	fd.density				= (density)?(density):((dimensions.x/PTMR) * (dimensions.y/PTMR));
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	body = world->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	body->SetTransform(toB2(position), 0.f);
}
void Entity::createCircularBody(float radius, int category, int mask, float density)
{
	b2BodyDef bd;
	bd.type					= b2_dynamicBody;
	bd.fixedRotation		= true;
	bd.allowSleep			= false;
	bd.position				= toB2(position);
	
	b2CircleShape shape;
	shape.m_radius			= radius / PTMR;

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.restitution			= 0.f;
	fd.friction				= 0.f;
	fd.density				= (density)?(density):(pow((radius/PTMR)*M_PI, 2));
	fd.userData				= this;
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	body = world->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	body->SetTransform(toB2(position), 0.f);
}
void Entity::deleteBody()
{
	if (body)
	{
		world->DestroyBody(body);
		body = NULL;
	}
}

b2Fixture* Entity::otherCollidingFixture(b2Contact *c, unsigned int catFlags)
{
	if (c && c->IsTouching())
	{
		b2Fixture *ret = NULL;
		if (c->GetFixtureA() != body->GetFixtureList())
		{
			ret = c->GetFixtureA();
		}
		else
		{
			ret = c->GetFixtureB();
		}

		if ((ret->GetFilterData().categoryBits & catFlags) != 0)
		{
			return ret;
		}
	}

	return NULL;
}