#include "Actor.h"
#include "TrollAI.h"


Actor::Actor(Pim::SpriteBatchNode *node, Pim::Vec2 pos)
{
	health		= 0.f;
	jumpForce	= 0.f;
	actorSheet	= node;
	position	= pos;
	body		= NULL;
	sensor		= NULL;
	useBatchNode(actorSheet);
}
Actor::~Actor()
{
}

void Actor::createRectangularBody(Pim::Vec2 dimensions, int category, int mask, float density)
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
	fd.filter.maskBits		= mask | LVLEDGE;

	body = world->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	body->SetTransform(toB2(position), 0.f);

	createSensor(-dimensions.y/PTMR);
}
void Actor::createCircularBody(float radius, int category, int mask, float density)
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
	fd.filter.maskBits		= mask | LVLEDGE;

	body = world->CreateBody(&bd);
	body->CreateFixture(&fd);
	body->SetUserData(this);

	body->SetTransform(toB2(position), 0.f);

	createSensor(-radius/PTMR);
}
void Actor::createSensor(float offsetY)
{
	b2BodyDef bd;
	bd.type					= b2_dynamicBody;
	bd.allowSleep			= false;
	bd.position				= body->GetPosition();

	b2PolygonShape shape;
	shape.SetAsBox(0.5f, 0.25f);

	b2FixtureDef fd;
	fd.shape				= &shape;
	fd.restitution			= 0.f;
	fd.friction				= 0.f;
	fd.density				= 0.001f;
	//fd.userData				= this;
	fd.filter.categoryBits	= SENSOR;
	fd.filter.maskBits		= GROUND;
	fd.isSensor				= true;

	sensor = world->CreateBody(&bd);
	sensor->CreateFixture(&fd);
	//sensor->SetUserData(this); Leave uncommented. Causes funky bugs.

	// Create a joint
	b2WeldJointDef jd;
	jd.bodyA = body;
	jd.bodyB = sensor;
	jd.localAnchorA = b2Vec2(0.f, offsetY);

	joint = world->CreateJoint(&jd);
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
	if (sensor && sensor->GetContactList())
	{
		for (auto c=sensor->GetContactList(); c; c=c->next)
		{
			int bodyCat = body->GetFixtureList()->GetFilterData().maskBits;

			if (otherCollidingFixture(c->contact, LVLEDGE|SENSOR|bodyCat))
			{
				return true;
			}
		}
	}
	return false;
}

void Actor::deleteBody()
{
	Entity::deleteBody();

	if (sensor)
	{
		world->DestroyBody(sensor);
		sensor	= NULL;
		joint	= NULL;
	}
	/*
	if (joint)
	{
		world->DestroyJoint(joint);
		joint = NULL;
	}
	*/
}