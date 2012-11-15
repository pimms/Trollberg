#pragma once
#include "Trollberg.h"

class Entity : public Pim::Sprite
{
public:
	Entity(std::string file);
	Entity();

	void setPosition(Pim::Vec2);
	void setPosition(b2Vec2);

	// The physics body
	b2Body		*body;

protected:
	// Keep a reference to the world
	b2World		*world;
};

