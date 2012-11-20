#pragma once
#include "Entity.h"

class Actor : public Entity
{
public:
	Actor(Pim::SpriteBatchNode *n, Pim::Vec2 p);
	~Actor();

	// Create a box body based on pixel dimensions. The body's center
	// will be the anchor point of the Actor.
	void createRectangularBody(Pim::Vec2 pixelDimensions, int category, int mask);

	// Create a circular body.
	void createCircularBody(float pixelRadius, int category, int mask);

	virtual void jump();		// Jump. Returns if Actor is airborne.
	virtual bool isGrounded();	// Checks whether or not the Actors is in contact with the ground

protected:
	Pim::SpriteBatchNode	*actorSheet;
	float		jumpForce;		// The force with which we jump. 60 is a good value.

	float		health;
};

