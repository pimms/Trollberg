#include "ColossusAI.h"
#include "Colossus.h"
#include "GameLayer.h"
#include "Player.h"


ColossusAI::ColossusAI(Colossus *t, Player *pl)
{
	troll		= t;
	colossus	= t;
	player		= pl;
	crushTimer	= 0.f;
	isCrushing	= false;
	hasDamaged	= false;
	hasPlayedSound = false;
}
ColossusAI::~ColossusAI()
{
}

void ColossusAI::update(float dt)
{
	// Is the player dead?
	if (GameLayer::isPlayerDead())
	{
		// If the tumbler is grounded, run away
		if (troll->isGrounded())
		{
			moveFromPlayer();
			colossus->rect = colossus->walkAnim.update(dt);
		}

		// Wait for the ground to hit it's feet
		else
		{
			colossus->rect = colossus->walkAnim.reset();
		}
		
		faceAwayPlayer();
		return;
	}

	if (abs(trollPlayerXDiff()) > 20.f && !isCrushing)
	{
		moveToPlayer();
		facePlayer();

		colossus->rect = colossus->walkAnim.update(dt);
	}
	else
	{
		updateCrush(dt);
	}
}
void ColossusAI::updateCrush(float dt)
{
	isCrushing  = true;
	crushTimer += dt;

	colossus->body->SetLinearVelocity(b2Vec2_zero);

	colossus->walkAnim.reset();
	colossus->rect = colossus->attackAnim.update(dt);

	if (crushTimer > 0.4f)
	{
		if (!hasPlayedSound)
		{
			Pim::Sound *s = new Pim::Sound;
			s->useCache("splat2");
			s->deleteWhenDone = true;
			s->play();
			hasPlayedSound = true;
		}

		colossus->createCrushSensor();
		crushCheck();

		if (VersionControl::getGLVersion() > LIGHT_GL_VERSION)
		{
			// Update the light
			float x = crushTimer - 0.4f;
			colossus->pld->radius = cosf((x-0.05f)*30.f) * 200.f;

			colossus->pld->position = Pim::Vec2(colossus->scale.x*-14.f, -5.f);
		}

		if (crushTimer > 0.5f)
		{
			colossus->destroyCrushSensor();

			isCrushing = false;
			hasDamaged = false;
			hasPlayedSound = false;
			crushTimer = 0.f;
			colossus->rect = colossus->attackAnim.reset();

			if (VersionControl::getGLVersion() > LIGHT_GL_VERSION)
			{
				colossus->pld->radius = 0;
			}
		}
	}
}

void ColossusAI::crushCheck()
{
	if (!hasDamaged)
	{
		if (colossus->otherCollidingFixture(colossus->crushSensor, PLAYER, true))
		{
			player->takeDamage(1);
			hasDamaged = true;
		}
	}
}