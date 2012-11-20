#pragma once
#include "Trollberg.h"

class Entity : public Pim::Sprite
{
public:
	Entity();

	void setPosition(Pim::Vec2);
	void setPosition(b2Vec2);

	void deleteBody();

	// The physics body
	b2Body		*body;

protected:
	// Keep a reference to the world
	b2World		*world;
};

