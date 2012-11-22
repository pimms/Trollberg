#pragma once
#include "Trollberg.h"

class Entity : public Pim::Sprite
{
public:
	Entity();

	void setPosition(Pim::Vec2);
	void setPosition(b2Vec2);

	// Create a box body.
	virtual void createRectangularBody(Pim::Vec2 pixelDimensions, int category, 
									   int mask, float density=0.f);

	// Create a circular body.
	virtual void createCircularBody(float pixelRadius, int category, 
									int mask, float density=0.f);

	virtual void deleteBody();

	// Returns the fixture in a TOUCHING collision which is not the fixture of the body-member.
	// If wanted, you may provide a category filter - the function will then only
	// return the other fixture if the category of that fixture matches the flag.
	b2Fixture* otherCollidingFixture(b2Contact *c, unsigned int catFlags = ~0);

	// The physics body
	b2Body		*body;

protected:
	// Keep a reference to the world
	b2World		*world;
};

