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

b2Body* Actor::createSensor(b2Body *attachBody, float offsetY, unsigned int maskBits)
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
	fd.filter.maskBits		= GROUND | maskBits;
	fd.isSensor				= true;

	b2Body *retSensor = world->CreateBody(&bd);
	retSensor->CreateFixture(&fd);

	// Create a joint
	b2WeldJointDef jd;
	jd.bodyA = attachBody;
	jd.bodyB = retSensor;
	jd.localAnchorA = b2Vec2(0.f, offsetY);

	joint = world->CreateJoint(&jd);

	return retSensor;
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
}