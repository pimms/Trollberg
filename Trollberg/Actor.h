#pragma once
#include "Entity.h"
#include "Animation.h"

class Actor : public Entity
{
public:
	Actor(Pim::SpriteBatchNode *n, Pim::Vec2 p);
	~Actor();
	
	// Create a sensor attached to the "body" object at "offsetY" distance. 
	// The sensor category is SENSOR, and it's mask GROUND by default. Pass any 
	// additional mask bits you see fit.
	b2Body* createSensor(b2Body *attachBody, float offsetY, unsigned int maskBits=0);

	virtual void jump();		// Jump. Returns if Actor is airborne.
	virtual bool isGrounded();	// Checks whether or not the Actors is in contact with the ground

	virtual void takeDamage(int dmg) = 0;

	virtual void deleteBody();

protected:

	b2Body					*sensor;		// The ground sensor
	b2Joint					*joint;			// The body/sensor joint

	Pim::SpriteBatchNode	*actorSheet;
	float					jumpForce;		// 60 is a good value
	int						health;	
};

