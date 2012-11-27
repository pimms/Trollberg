#include "Colossus.h"
#include "ColossusAI.h"
#include "Player.h"
#include "TrollControl.h"

#define C_TIMETODIE 2.f
#define C_FADETIME 6.f

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

	attackAnim.firstFramePos		= Pim::Vec2(160.f, 64.f);
	attackAnim.frameWidth			= 40;
	attackAnim.frameHeight			= 40;
	attackAnim.frameTime			= 0.1f;
	attackAnim.framesInAnimation	= 5;
	attackAnim.horizontalFrames		= 5;
	attackAnim.totalFrames			= 5;

	deathAnim.firstFramePos			= Pim::Vec2(0.f, 104.f);
	deathAnim.frameWidth			= 40;
	deathAnim.frameHeight			= 40;
	deathAnim.frameTime				= 0.25f;
	deathAnim.framesInAnimation		= 8;
	deathAnim.horizontalFrames		= 8;
	deathAnim.totalFrames			= 8;

	rect							= walkAnim.frameIndex(0);

	scoreValue						= 100;
	crushSensor						= NULL;
	health							= 200;
	b2offset						= Pim::Vec2(0.f, 10.f);
	ai								= new ColossusAI(this, pl);
	walkSpeed						= 4.f;
}

void Colossus::createPhysics()
{
	body = createCircularBody(9, TROLLS_C, TROLLS_C | PLAYER | GROUND | SENSOR);
	sensor = createSensor(body, -8.f/PTMR);

	body2 = createCircularBody(8, TROLLS_C, TROLLS_C | PLAYER | GROUND | SENSOR);

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
void Colossus::createLight()
{
	pld = new Pim::PreloadLightDef;
	pld->radius = 0;
	
	getParentLayer()->addLight(this, pld, "CCrush");
}

void Colossus::createCrushSensor()
{
	if (!crushSensor)
	{
		crushSensor = createRectangularBody(Pim::Vec2(15.f, 10.f), TROLLS, PLAYER, 0.f);
		crushSensor->GetFixtureList()->SetSensor(true);

		b2Vec2 offset((scale.x*-14)/PTMR, -5/PTMR);
		crushSensor->SetTransform(crushSensor->GetPosition()+offset, 0.f);
	}
}
void Colossus::destroyCrushSensor()
{
	if (crushSensor)
	{
		world->DestroyBody(crushSensor);
		crushSensor = NULL;
	}
}

void Colossus::deleteBody()
{
	Troll::deleteBody();
	destroyCrushSensor();
}