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
	RTReleaseRequired			= false;

	body = createCircularBody(6.f, PLAYER, GROUND | TROLLS | LVLEDGE);
	sensor = createSensor(body, -4.f/PTMR, TROLLS);

	// Create all the weapons
	allWeapons[0]				= Weapon::createWeapon(actorSheet, REVOLVER);
	allWeapons[1]				= Weapon::createWeapon(actorSheet, SHOTGUN);
	allWeapons[2]				= Weapon::createWeapon(actorSheet, SNIPER);

	// Child all the weapons
	addChild(allWeapons[0]);
	addChild(allWeapons[1]);
	addChild(allWeapons[2]);

	// Hide weapons 1 and 2
	allWeapons[1]->hidden = true;
	allWeapons[2]->hidden = true;
	
	curWep = REVOLVER;
	weapon = allWeapons[0];

	listenFrame();
	listenInput();			// calls listenMouse() listenKeys()
	listenController();		// 360pad support
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
	if (evt.isKeyDown(Pim::KeyEvent::K_SPACE))
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
	if (evt.isKeyFresh(Pim::MouseEvent::MBTN_RIGHT))
	{
		weapon->fire();
	}
}
void Player::controllerEvent(Pim::ControllerEvent &evt)
{
	// Movement - left stick
	// Let keyboard movement override controller input
	//if (abs(velX < 1.f))
	{
		Pim::Vec2 ls = evt.leftStick();
		velX = ls.x * 500.f;
	}

	// Jumping - A or left trigger
	if (evt.isKeyDown(Pim::ControllerEvent::X_A) || 
		evt.leftTrigger() > 0.6f)
	{
		jump();
	}
	
	// Firing the gun - right trigger
	if (evt.rightTrigger() > 0.6f && !RTReleaseRequired)
	{
		weapon->fire();
		RTReleaseRequired = true;
	}
	else if (evt.rightTrigger() <= 0.4f)
	{
		RTReleaseRequired = false;
	}

	// Weapon changing - left and right bumper
	if (evt.isKeyFresh(Pim::ControllerEvent::X_LB))
	{
		setActiveWeapon((curWep-1) % 3);
	}
	else if (evt.isKeyFresh(Pim::ControllerEvent::X_RB))
	{
		setActiveWeapon((curWep+1) % 3);
	}

	// X Y B - Revolver Shotgun Sniper
	if (evt.isKeyFresh(Pim::ControllerEvent::X_X))
	{
		setActiveWeapon(REVOLVER);
	}
	else if (evt.isKeyFresh(Pim::ControllerEvent::X_Y))
	{
		setActiveWeapon(SHOTGUN);
	}
	else if (evt.isKeyFresh(Pim::ControllerEvent::X_B))
	{
		setActiveWeapon(SNIPER);
	}

	// Weapon rotation
	Pim::Vec2 rs = evt.rightStick();
	if (rs.length() >= 0.1f)	// Ignore idle 0-degrees
	{
		weapon->rotation = 90 - rs.angleBetween360(Pim::Vec2(0.f, 1.f));
		weapon->setMirrored(rs.x < 0.f);
	}
}

void Player::takeDamage(int damage)
{
	GameLayer::getSingleton()->vibrate(1.f, 1.f, 1.f);

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

void Player::setActiveWeapon(int wep)
{
	if (wep < 0)
	{
		wep += 3;
	}

	for (int i=0; i<3; i++)
	{
		if (i != wep)
		{
			allWeapons[i]->hidden = true;
		}
		else
		{
			// keep the current rotation
			allWeapons[i]->rotation = weapon->rotation;
			allWeapons[i]->setMirrored(weapon->scale.y == -1.f);

			allWeapons[i]->hidden = false;
			weapon = allWeapons[i];
			curWep = wep;

			HUDLayer::getSingleton()->setSelectedWeapon(i);
		}
	}
}
