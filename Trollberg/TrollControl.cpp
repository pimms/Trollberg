#include "Trollberg.h"

#include "Weapon.h"
#include "GameLayer.h"
#include "TrollControl.h"
#include "Player.h"

#include "Troll.h"
#include "Slinker.h"
#include "Tumbler.h"
#include "Colossus.h"

#include "GameScene.h"

TrollControl* TrollControl::singleton = NULL;

TrollControl::TrollControl(Pim::SpriteBatchNode *batch, GameLayer *l, 
						   Player *pl, float lvlWidth, int lvl)
{

	singleton = this;

	actorSheet				= batch;
	player					= pl;
	levelWidth				= lvlWidth;
	levelIdx				= lvl-1;
	gameLayer				= l;
	nextTroll				= NONE;

	player					= pl;

	trollSpawnTimer			= 0.f;
	trollsAlive				= 0;
	maxTrollsAlive			= 15;

	if (levelIdx == 0)
	{
		trollSpawnChance[0] = 60;
		trollSpawnChance[1] = 30;
		trollSpawnChance[2] = 10;

		trollSpawnCost[0]	= 0.5f;
		trollSpawnCost[1]	= 1.f;
		trollSpawnCost[2]	= 1.5f;
	}
	else if (levelIdx == 1)
	{
		trollSpawnChance[0] = 40;
		trollSpawnChance[1] = 40;
		trollSpawnChance[2] = 20;

		trollSpawnCost[0]	= 0.5f;
		trollSpawnCost[1]	= 0.5f;
		trollSpawnCost[2]	= 1.f;
	}
	else if (levelIdx == 2)
	{
		trollSpawnChance[0] = 20;
		trollSpawnChance[1] = 40;
		trollSpawnChance[2] = 40;

		trollSpawnCost[0]	= 0.25f;
		trollSpawnCost[1]	= 0.25f;
		trollSpawnCost[2]	= 0.5f;
	}
}
TrollControl::~TrollControl(void)
{
	singleton = NULL;
}

void TrollControl::update(float dt)
{
	trollSpawnTimer += dt;

	if (nextTroll == NONE)
	{
		nextTroll = getNextTrollType();
	}

	if (trollSpawnTimer >= trollSpawnCost[nextTroll])
	{
		spawnTroll();
		nextTroll = NONE;
	}
}

void TrollControl::spawnTroll()
{
	if (nextTroll == SLINKER)
	{
		addSlinker();
		trollsAlive++;
	}
	else if (nextTroll == TUMBLER)
	{
		addTumbler();
		trollsAlive++;
	}
	else if (nextTroll == COLOSSUS)
	{
		addColossus();
		trollsAlive++;
	}
}

TrollID TrollControl::getNextTrollType()
{
	if (trollsAlive <= maxTrollsAlive)
	{
		int ran = rand() % 100 + 1;

		int soFar = 0;
		for (int i=0; i<3; i++)
		{
			if (ran <= trollSpawnChance[i] + soFar)
			{
				return (TrollID)i;
			}
		
			soFar += trollSpawnChance[i];
		}
		printf("ERROR IN SELECTING TROLL ID.\n");
	}
	return NONE;
}
void TrollControl::addSlinker()
{
	Slinker *sl = new Slinker(player, actorSheet, Pim::Vec2(getTrollSpawnX(), 150.f));
	gameLayer->addChild(sl);
	trollSpawnTimer -= trollSpawnCost[levelIdx];
}
void TrollControl::addTumbler()
{
	Tumbler *tl = new Tumbler(player, actorSheet, Pim::Vec2(getTrollSpawnX(), 150.f));
	gameLayer->addChild(tl);
	trollSpawnTimer -= trollSpawnCost[levelIdx];
}
void TrollControl::addColossus()
{
	Colossus *c = new Colossus(player, actorSheet, Pim::Vec2(getTrollSpawnX(), 150.f));
	gameLayer->addChild(c);
	c->createLight();
	trollSpawnTimer -= trollSpawnCost[levelIdx];
}

float TrollControl::getTrollSpawnX()
{
	// Return a value +- 90 pixels from the screen edge on either side
	int	dir		= ((rand() % 2) * 2) - 1; 
	int screenX = player->position.x + gameLayer->position.x;

	if (dir == 1)
	{
		return player->position.x + (SCREENWIDTH - screenX) + 40 + rand()%51;
	}
	else
	{
		return player->position.x - screenX - 40 - rand()%51;
	}
}

void TrollControl::trollKilled()
{
	trollsAlive--;
}
