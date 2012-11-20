#include "Actor.h"


Actor::Actor(Pim::SpriteBatchNode *node, Pim::Vec2 pos)
{
	jumpForce	= 0.f;
	actorSheet	= node;
	position	= pos;
	useBatchNode(actorSheet);
}
Actor::~Actor()
{
}

void Actor::createRectangularBody(Pim::Vec2 dimensions, int category, int mask)
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
	fd.density				= (dimensions.x/PTMR) * (dimensions.y/PTMR);
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	body = world->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	body->SetTransform(toB2(position), 0.f);
}
void Actor::createCircularBody(float radius, int category, int mask)
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
	fd.density				= pow((radius/PTMR)*M_PI, 2);
	fd.filter.categoryBits	= category;
	fd.filter.maskBits		= mask;

	body = world->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	body->SetTransform(toB2(position), 0.f);
}

void Actor::jump()
{
	if (isGrounded())
	{
		body->SetLinearVelocity(b2Vec2_zero);
		body->ApplyLinearImpulse(b2Vec2(0.f, jumpForce), body->GetPosition());
	}
}
bool Actor::isGrounded()
{
	if (body && body->GetContactList())
	{
		for (auto c=body->GetContactList(); c; c=c->next)
		{
			if (!c->contact->IsTouching())
				continue;

			int ABCat;
			int bodyCat = body->GetFixtureList()->GetFilterData().maskBits;
			b2Fixture *fixA = c->contact->GetFixtureA();
			b2Fixture *fixB = c->contact->GetFixtureB();
			
			if (fixA->GetBody() != body)
			{
				ABCat = fixA->GetFilterData().categoryBits;
				if ((ABCat & LVLEDGE) == 0 && (ABCat & bodyCat) != 0)
					return true;
			}
			if (fixB->GetBody() != body)
			{
				ABCat = fixB->GetFilterData().categoryBits;
				if ((ABCat & LVLEDGE) == 0 && (ABCat & bodyCat) != 0)
					return true;
			}
		}
	}
	return false;
}