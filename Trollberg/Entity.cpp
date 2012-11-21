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

		if (ret->GetFilterData().categoryBits & catFlags != 0)
			return ret;
	}

	return NULL;
}