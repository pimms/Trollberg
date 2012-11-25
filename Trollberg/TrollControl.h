#pragma once

// Forward declarations
class GameLayer;
class Weapon;
class Player;
class Troll;

enum TrollID 
{
	SLINKER,
	TUMBLER,
	COLOSSUS,
	NONE,
};

class TrollControl
{
public:
	static TrollControl* getSingleton(){ return singleton; };

	TrollControl(Pim::SpriteBatchNode *batch, GameLayer *layer, 
				Player *pl, float lvlWidth, int lvl);
	virtual ~TrollControl(void);

	// Called from the GameLayer
	void update(float dt);

	void spawnTroll();

	TrollID getNextTrollType();
	float getTrollSpawnX();

	void addSlinker();
	void addTumbler();
	void addColossus();

	void trollKilled();

private:
	static TrollControl		*singleton;

	Pim::SpriteBatchNode	*actorSheet;
	Player					*player;
	GameLayer				*gameLayer;

	float					levelWidth;
	int						levelIdx;

	TrollID					nextTroll;
	float					trollSpawnTimer;
	int						trollsAlive;
	int						maxTrollsAlive;

	int						trollSpawnChance[3];
	float					trollSpawnCost[3];
};

