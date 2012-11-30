#include "Player.h"
#include "GameLayer.h"
#include "HUDLayer.h"

#define PL_TIMETODIE 6.9f
#define PL_HEALTIME 10.0f

Player::Player(Pim::SpriteBatchNode *node, Pim::Vec2 pos)
	: Actor(node, pos)
{
	healthTimer = 0;

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
	keyMovement					= false;
	
	// jetpack
	jpFuel						= 1.f;
	usingJp						= false;
	reqJpRelease				= false;
	jpFire = new Pim::Sprite;
	jpFire->useBatchNode(actorSheet);
	jpFire->rect = Pim::Rect(224.f, 0.f, 3.f, 3.f);
	jpFire->anchor = Pim::Vec2(0.f, 0.f);
	jpFire->position = Pim::Vec2(-6.f, -6.f);
	addChild(jpFire);

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
	allWeapons[1]->hidden		= true;
	allWeapons[2]->hidden		= true;
	
	curWep						= REVOLVER;
	weapon						= allWeapons[0];

	listenFrame();
	listenInput();			// calls listenMouse() listenKeys()
	listenController();		// Xboxcontroller input
}
Player::~Player()
{
}

void Player::createLight()
{
	lightDef = new Pim::SmoothLightDef;
	lightDef->radius			= 220;
	lightDef->castShadows		= true;
	getParentLayer()->addLight(this, lightDef);

	lightPosition = Pim::Vec2(0.f, -4.f);
}

void Player::keyEvent(Pim::KeyEvent &evt)
{

	if (evt.isKeyDown(Pim::KeyEvent::K_SPACE))
	{
		jump();
	}

	if (evt.isKeyDown(Pim::KeyEvent::K_A))
	{
		velX = -500.f;
		keyMovement = true;
	}
	else if (evt.isKeyDown(Pim::KeyEvent::K_D))
	{
		velX = 500.f;
		keyMovement = true;
	}
	else 
	{
		velX = 0.f;
		keyMovement = false;
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

	if (evt.isKeyDown(Pim::KeyEvent::K_SHIFT))
	{
		usingJp = !reqJpRelease;
	}
	else
	{
		reqJpRelease = false;
		usingJp = false;
	}
} 
void Player::mouseEvent(Pim::MouseEvent &evt)
{
	mEvt = &evt;

	ShowCursor(true);

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
	if (!keyMovement)
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
		mEvt = NULL;	// Ignore mouse input
		ShowCursor(false);

		weapon->rotation = 90 - rs.angleBetween360(Pim::Vec2(0.f, 1.f));
		weapon->setMirrored(rs.x < 0.f);
	}

	// Jetpack
	if (evt.isKeyDown(Pim::ControllerEvent::X_LS) ||
		evt.isKeyDown(Pim::ControllerEvent::X_RS))
	{
		usingJp = !reqJpRelease;
	}
	else
	{
		reqJpRelease = false;
		usingJp = false;
	}
}

void Player::takeDamage(int damage)
{
	GameLayer::getSingleton()->vibrate(1.f, 1.f, 1.f);

	if (!dead)
	{
		health -= damage;
		HUDLayer::getSingleton()->setPlayerHealth(health);
		HUDLayer::getSingleton()->playerTookDamage();

		if (health <= 0)
		{
			weapon->hidden = true;

			dead = true;
			deathTimer = 0.f;

			unlistenInput();
			unlistenController();
		}
	}
}

void Player::update(float dt)
{
	if (!dead)
	{
		if(health < 3)
		{
			healthTimer += dt;
		}

		if(healthTimer > PL_HEALTIME && health < 3)
		{
 			health ++;
			HUDLayer::getSingleton()->setPlayerHealth(health);
			healthTimer = 0;
		}


		// Update the weapon
		updateWeapon();					

		// Face the aimed direction
		scale.x = weapon->scale.y;	
		jpFire->position.x = (scale.x > 0.f) ? (-6.f) : (3.f);
		b2offset.x = 2*scale.x;

		// Update the velocity of the player's body
		b2Vec2 vel(velX*dt, body->GetLinearVelocity().y);
		body->SetLinearVelocity(vel);

		// Update the jetpack
		if (usingJp && jpFuel >= 0.f)
		{
			jpFuel -= dt * 2.f;
			jpFire->hidden = false;
			body->ApplyForce(b2Vec2(0.f, 10000.f*dt), body->GetPosition());

			if (jpFuel < 0.f)
			{
				reqJpRelease = true;
			}
		}
		else
		{
			jpFuel += dt * 0.5f;
			if (jpFuel > 1.f)
			{
				jpFuel = 1.f;
			}

			jpFire->hidden = true;
		}

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
			rect = walkAnim.frameIndex(3);
		}
	}
	else
	{
		jpFire->hidden = true;

		if (isGrounded())
		{
			body->SetLinearVelocity(b2Vec2_zero);
		}

		if (lightDef)
		{
			lightDef->radius = 120 - (deathTimer*24.f);
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
				// Slow down the animation for the last two frames
				deathAnim.frameTime = 2.f;
			}
			rect		= deathAnim.update(dt);
			deathTimer += dt;
		}
		else
		{
			rect = deathAnim.frameIndex(deathAnim.totalFrames-1);
			HUDLayer::getSingleton()->playerDead = true;
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
