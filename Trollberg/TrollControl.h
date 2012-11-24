#pragma once

// Forward declarations
class GameLayer;
class Weapon;
class Player;
class Troll;

class TrollControl
{
public:
	static TrollControl* getSingleton(){ return singleton; };

	TrollControl(Pim::SpriteBatchNode *batch, GameLayer *layer, 
				Player *pl, float lvlWidth, int lvl);
	virtual ~TrollControl(void);

	// Called from the GameLayer
	void update(float dt);
	void trollKilled();

private:
	friend class Weapon;

	static TrollControl		*singleton;

	std::vector<Troll*>		trolls;

	Pim::SpriteBatchNode	*actorSheet;
	Player					*player;
	GameLayer				*layer;
	float					levelWidth;
	int						levelNum;

	Player					*pl;

	float trollSpawnTimer;
	int trollsToSpawn;
	int trollsOnScreen;
	int maxTrollsOnScreen;
	float trollTimeBetweenSpawn;

	float ranDirr;
	float spawnX;

	int trollSpawnChance[4][4];
	float trollSpawnDelay[4][4];

	void spawnTroll();

};

