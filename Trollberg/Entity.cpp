#include "Entity.h"
#include "GameScene.h"


Entity::Entity(std::string file)
	: Pim::Sprite(file)
{
	body = NULL;
	world = GameScene::getWorld();
}
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