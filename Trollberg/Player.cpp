#include "Player.h"


Player::Player(Pim::SpriteBatchNode *node, Pim::Vec2 pos)
	: Actor()
{
	// Prepare the animation object
	anim.frameWidth = 14;
	anim.frameHeight = 14;
	anim.firstFramePos = Pim::Vec2(0,0);
	anim.framesInAnimation = 4;
	anim.totalFrames = 5;
	anim.horizontalFrames = 5;

	useBatchNode(node);
	rect		= anim.frameIndex(0);

	position	= pos;
	jumpForce	= 70.f;
	velX		= 0.;

	createCircularBody(6.f, PLAYER, GROUND | TROLLS);

	listenFrame();
	listenKeys();
}
Player::~Player()
{
}

void Player::createLight()
{
	Pim::SmoothLightDef *ld = new Pim::SmoothLightDef;
	ld->radius			= 80;
	ld->innerColor.a	= 0.7f;
	ld->castShadows		= false;
	getParentLayer()->addLight(this, ld);
}

void Player::update(float dt)
{
	b2Vec2 vel(velX*dt, body->GetLinearVelocity().y);
	body->SetLinearVelocity(vel);

	if (velX < 0.f)
		scale.x = -1.f;
	else if (velX > 0.f)
		scale.x = 1.f;

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
void Player::keyEvent(Pim::KeyEvent &evt)
{
	if (evt.isKeyFresh(Pim::KeyEvent::K_SPACE))
		jump();

	if (evt.isKeyDown(Pim::KeyEvent::K_A))
		velX = -500.f;
	else if (evt.isKeyDown(Pim::KeyEvent::K_D))
		velX = 500.f;
	else 
		velX = 0.f;
}