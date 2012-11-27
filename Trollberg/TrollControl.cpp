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

	actorSheet	= batch;
	player		= pl;
	levelWidth	= lvlWidth;
	levelNum	= lvl;
	layer		= l;

	this->pl = pl;

	trollSpawnTimer = 0.f;
	trollsToSpawn = 100000;
	trollsOnScreen = 0;
	maxTrollsOnScreen = 500;
	trollTimeBetweenSpawn = 0.5f;

	//lvl 1
	trollSpawnChance[1][1] = 60;
	trollSpawnChance[1][2] = 30;
	trollSpawnChance[1][3] = 10;

	//lvl 2
	trollSpawnChance[2][1] = 40;
	trollSpawnChance[2][2] = 40;
	trollSpawnChance[2][3] = 20;

	//lvl3
	trollSpawnChance[3][1] = 20;
	trollSpawnChance[3][2] = 40;
	trollSpawnChance[3][3] = 40;

	//extra delay when spawning one type of troll:
	//lvl 1
	trollSpawnDelay[1][1] = 0.f;
	trollSpawnDelay[1][2] = 0.f;
	trollSpawnDelay[1][3] = 0.f;

	//lvl 2
	trollSpawnDelay[2][1] = 0.f;
	trollSpawnDelay[2][2] = 1.f;
	trollSpawnDelay[2][3] = 2.f;

	//lvl3
	trollSpawnDelay[3][1] = 0.f;
	trollSpawnDelay[3][2] = 0.f;
	trollSpawnDelay[3][3] = 1.f;
}
TrollControl::~TrollControl(void)
{
	singleton = NULL;
}

	actorSheet				= batch;
	player					= pl;
	levelWidth				= lvlWidth;
	levelIdx				= lvl-1;
	gameLayer				= l;
	nextTroll				= NONE;

	player					= pl;

	trollSpawnTimer			= 0.f;
	trollsAlive				= 0;
	maxTrollsAlive			= 20;

	if (levelIdx == 0)
	{
		trollSpawnChance[0] = 60;
		trollSpawnChance[1] = 30;
		trollSpawnChance[2] = 10;

		trollSpawnCost[0]	= 0.5f;
		trollSpawnCost[1]	= 0.5f;
		trollSpawnCost[2]	= 2.f;
	}
	else if (levelIdx == 1)
	{
		trollSpawnChance[0] = 40;
		trollSpawnChance[1] = 40;
		trollSpawnChance[2] = 20;

		trollSpawnCost[0]	= 0.1f;
		trollSpawnCost[1]	= 1.f;
		trollSpawnCost[2]	= 2.f;
	}
	else if (levelIdx == 2)
	{
		trollSpawnChance[0] = 20;
		trollSpawnChance[1] = 40;
		trollSpawnChance[2] = 40;

		trollSpawnCost[0]	= 0.1f;
		trollSpawnCost[1]	= 0.2f;
		trollSpawnCost[2]	= 1.f;
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
	}
	else if (nextTroll == TUMBLER)
	{
		addTumbler();
	}
	else if (nextTroll == COLOSSUS)
	{
		addColossus();
	}

	trollsAlive++;
}

TrollID TrollControl::getNextTrollType()
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
