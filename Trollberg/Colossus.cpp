#include "Colossus.h"
#include "ColossusAI.h"
#include "Player.h"


Colossus::Colossus(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p)
	: Troll(b, p)
{
	createPhysics();

	walkAnim.firstFramePos			= Pim::Vec2(0.f, 64.f);
	walkAnim.frameWidth				= 40;
	walkAnim.frameHeight			= 40;
	walkAnim.frameTime				= 0.2f;
	walkAnim.framesInAnimation		= 4;
	walkAnim.horizontalFrames		= 4;
	walkAnim.totalFrames			= 4;

	crushAnim.firstFramePos			= Pim::Vec2(160.f, 64.f);
	crushAnim.frameWidth			= 40;
	crushAnim.frameHeight			= 40;
	crushAnim.frameTime				= 0.2f;
	crushAnim.framesInAnimation		= 5;
	crushAnim.horizontalFrames		= 5;
	crushAnim.totalFrames			= 5;

	deathAnim.firstFramePos			= Pim::Vec2(160.f, 64.f);
	deathAnim.frameWidth			= 40;
	deathAnim.frameHeight			= 40;
	deathAnim.frameTime				= 0.3f;
	deathAnim.framesInAnimation		= 8;
	deathAnim.horizontalFrames		= 8;
	deathAnim.totalFrames			= 8;

	rect							= walkAnim.frameIndex(0);

	health							= 200.f;
	deathTimer						= 0.f;
	dead							= false;
	b2offset						= Pim::Vec2(0.f, 10.f);
	ai								= new ColossusAI(this, pl);
	walkSpeed						= 4.f;
}
Colossus::~Colossus()
{
}

void Colossus::createPhysics()
{
	body = createCircularBody(9, TROLLS, TROLLS | PLAYER | GROUND | SENSOR);
	sensor = createSensor(body, -8.f/PTMR);

	body2 = createCircularBody(8, TROLLS, TROLLS | PLAYER | GROUND | SENSOR);

	// Joint the two bodies together. WeldJoint didn't work. WHAT THE FLYING FUCK.
	// Using a revolute joint instead, turning off rotation.
	b2RevoluteJointDef jd;
	jd.Initialize(body, body2, body->GetPosition());
	jd.localAnchorB = b2Vec2(0,-2);
	jd.collideConnected = false;
	jd.lowerAngle = 0.f;
	jd.upperAngle = 0.f;

	world->CreateJoint(&jd);
}

void Colossus::update(float dt)
{
	ai->update(dt);
	
	b2offset.x = -10 * scale.x;
}
