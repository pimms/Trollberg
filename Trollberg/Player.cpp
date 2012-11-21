#include "Player.h"
#include "HUDLayer.h"

Player::Player(Pim::SpriteBatchNode *node, Pim::Vec2 pos)
	: Actor(node, pos)
{
	// Prepare the animation object
	anim.frameWidth			= 14;
	anim.frameHeight		= 14;
	anim.firstFramePos		= Pim::Vec2(0,0);
	anim.framesInAnimation	= 4;
	anim.totalFrames		= 5;
	anim.horizontalFrames	= 5;
	rect					= anim.frameIndex(0);
	
	health					= 3;
	jumpForce				= 70.f;
	velX					= 0.;
	mEvt					= NULL;

	createCircularBody(6.f, PLAYER, GROUND | TROLLS);

	weapon = Weapon::createWeapon(actorSheet, LIGHT_RIFLE);
	addChild(weapon);

	listenFrame();
	listenInput();	// calls listenMouse() listenKeys()
}
Player::~Player()
{
}

void Player::createLight()
{
	Pim::SmoothLightDef *ld = new Pim::SmoothLightDef;
	ld->radius			= 600;
	ld->castShadows		= false;
	getParentLayer()->addLight(this, ld);
}

void Player::keyEvent(Pim::KeyEvent &evt)
{
	if (evt.isKeyFresh(Pim::KeyEvent::K_SPACE))
	{
		jump();
	}

	if (evt.isKeyDown(Pim::KeyEvent::K_A))
	{
		velX = -500.f;
	}
	else if (evt.isKeyDown(Pim::KeyEvent::K_D))
	{
		velX = 500.f;
	}
	else 
	{
		velX = 0.f;
	}
} 
void Player::mouseEvent(Pim::MouseEvent &evt)
{
	mEvt = &evt;

	if (evt.isKeyDown(Pim::MouseEvent::MBTN_LEFT))
		weapon->fire();

	/*
	if (evt.isKeyDown(Pim::MouseEvent::MBTN_RIGHT))
		weapon->altFire();
	*/
}

void Player::takeDamage(int damage)
{
	health -= damage;

	HUDLayer::getSingleton()->setPlayerHealth(health);
}

void Player::update(float dt)
{
	// Update the weapon
	updateWeapon();					

	// Face the aimed direction
	scale.x = weapon->scale.y;		

	// Update the velocity of the player's body
	b2Vec2 vel(velX*dt, body->GetLinearVelocity().y);
	body->SetLinearVelocity(vel);

	// Update the animation
	if (isGrounded())
	{
		if (velX != 0.f)
			rect = anim.update(dt);
		else 
			rect = anim.reset();
	}
	else
	{
		anim.reset();
		rect = anim.frameIndex(4);
	}
}
void Player::updateWeapon()
{
	if (mEvt)
	{
		Pim::Vec2 p = position + parent->position;	// Screen coord of player
		Pim::Vec2 d = p - mEvt->getPosition();		// difference
		weapon->rotation = -d.angleBetween360(Pim::Vec2(-1.f, 0.f));

		weapon->setMirrored(p.x > mEvt->getPosition().x);
	}
}
