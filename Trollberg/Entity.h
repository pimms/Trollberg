#pragma once
#include "Trollberg.h"

class Entity : public Pim::Sprite
{
public:
	Entity();

	void setPosition(Pim::Vec2);
	void setPosition(b2Vec2);

	// Overriden so the b2offset can be taken into account
	void draw();

	// Create a box body.
	virtual b2Body* createRectangularBody(Pim::Vec2 pixelDimensions, int category, 
									   int mask, float density=0.f);

	// Create a circular body.
	virtual b2Body* createCircularBody(float pixelRadius, int category, 
									int mask, float density=0.f);

	virtual void deleteBody();

	// Returns the fixture in a TOUCHING collision which is not the fixture of the body-member.
	// If wanted, you may provide a category filter - the function will then only
	// return the other fixture if the category of that fixture matches the flag.
	b2Fixture* otherCollidingFixture(b2Contact *c, unsigned int catFlags = ~0);

	// Checks whether a specified body is touching anything of the passed flags.
	b2Fixture* otherCollidingFixture(b2Body *b, unsigned int catFlags = ~0, bool isSensor=false);

	// The physics body
	b2Body		*body;

	// The offset (in pixels) of the box2d body
	Pim::Vec2	b2offset;

	// Ignore the rotation of the body
	bool		ignoreb2Rotation;

protected:
	// Keep a reference to the world
	b2World		*world;
};

