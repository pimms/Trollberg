#include "Player.h"
#include "GameLayer.h"
#include "HUDLayer.h"

#define PL_TIMETODIE 6.9f

Player::Player(Pim::SpriteBatchNode *node, Pim::Vec2 pos)
	: Actor(node, pos)
{
	// Prepare the animation object
	walkAnim.frameWidth			= 14;
	walkAnim.frameHeight		= 14;
	walkAnim.firstFramePos		= Pim::Vec2(69,0);
	walkAnim.framesInAnimation	= 4;
	walkAnim.totalFrames		= 5;
	walkAnim.horizontalFrames	= 5;
	walkAnim.frameTime			= 0.1f;
	rect						= walkAnim.frameIndex(0);

	deathAnim.frameWidth		= 14;
	deathAnim.frameHeight		= 14;
	deathAnim.firstFramePos		= Pim::Vec2(0,0);
	deathAnim.framesInAnimation	= 5;
	deathAnim.totalFrames		= 5;
	deathAnim.horizontalFrames	= 5;
	deathAnim.frameTime			= 1.f;
	
	health						= 3;
	jumpForce					= 70.f;
	velX						= 0.;
	mEvt						= NULL;
	dead						= false;
	deathTimer					= 0.f;

	body = createCircularBody(6.f, PLAYER, GROUND | TROLLS | LVLEDGE);
	sensor = createSensor(body, -4.f/PTMR);

	weapon = Weapon::createWeapon(actorSheet, SNIPER);
	addChild(weapon);

	listenFrame();
	listenInput();	// calls listenMouse() listenKeys()
}
Player::~Player()
{
}

void Player::createLight()
{
	lightDef = new Pim::SmoothLightDef;
	lightDef->radius			= 600;
	lightDef->castShadows		= false;
	getParentLayer()->addLight(this, lightDef);

	lightPosition = Pim::Vec2(0.f, -4.f);
}

void Player::keyEvent(Pim::KeyEvent &evt)
{
	if (evt.isKeyFresh(Pim::KeyEvent::K_Q))
	{
		exit(0);
	}
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

	if (evt.isKeyFresh(Pim::KeyEvent::K_1))
	{
		setActiveWeapon(REVOLVER);
	}
	if (evt.isKeyFresh(Pim::KeyEvent::K_2))
	{
		setActiveWeapon(SHOTGUN);
	}
	if (evt.isKeyFresh(Pim::KeyEvent::K_3))
	{
		setActiveWeapon(SNIPER);
	}
} 
void Player::mouseEvent(Pim::MouseEvent &evt)
{
	mEvt = &evt;

	if (evt.isKeyFresh(Pim::MouseEvent::MBTN_LEFT))
	{
		weapon->fire();
	}
	if (evt.isKeyDown(Pim::MouseEvent::MBTN_RIGHT))
	{
		weapon->fire();
	}
}

void Player::takeDamage(int damage)
{
	//FUCK DAMAGE!
	return;
	if (!dead)
	{
		health -= damage;
		HUDLayer::getSingleton()->setPlayerHealth(health);

		if (health <= 0)
		{
			weapon->hidden = true;

			dead = true;
			deathTimer = 0.f;

			unlistenInput();
		}
	}
}

void Player::update(float dt)
{
	if (!dead)
	{
		// Update the weapon
		updateWeapon();					

		// Face the aimed direction
		scale.x = weapon->scale.y;	
		b2offset.x = 2*scale.x;

		// Update the velocity of the player's body
		b2Vec2 vel(velX*dt, body->GetLinearVelocity().y);
		body->SetLinearVelocity(vel);

		// Update the animation
		if (isGrounded())
		{
			if (velX != 0.f)
				rect = walkAnim.update(dt);
			else 
				rect = walkAnim.reset();
		}
		else
		{
			walkAnim.reset();
			rect = walkAnim.frameIndex(2);
		}
	}
	else
	{
		if (isGrounded())
		{
			body->SetLinearVelocity(b2Vec2_zero);
		}

		if (lightDef)
		{
			lightDef->radius -= dt * 50;
			if (lightDef->radius <= 0)
			{
				getParentLayer()->removeLight(this);
				lightDef = NULL;
			}
		}

		if (deathTimer < PL_TIMETODIE)
		{
			if (deathAnim.curFrame == 3)
			{
				deathAnim.frameTime = 2.f;
			}
			rect		= deathAnim.update(dt);
			deathTimer += dt;
		}
	}
}
void Player::updateWeapon()
{
	if (mEvt)
	{
		// Screen coord of player
		Pim::Vec2 p = position + parent->position;
		
		// difference
		Pim::Vec2 d = p - mEvt->getPosition();		

		weapon->rotation = -d.angleBetween360(Pim::Vec2(-1.f, 0.f));
		weapon->setMirrored(p.x > mEvt->getPosition().x);
	}
}

void Player::setActiveWeapon(WeaponID wep)
{
	weapon->getParent()->removeChild(weapon, true);
	weapon = Weapon::createWeapon(actorSheet, wep);
	addChild(weapon);
}
