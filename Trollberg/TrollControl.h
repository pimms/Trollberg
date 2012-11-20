#pragma once

// Forward declarations
class GameLayer;
class Weapon;
class Player;
class Troll;

class TrollControl
{
public:
	static TrollControl* getSingleton();

	TrollControl(Pim::SpriteBatchNode *batch, GameLayer *layer, 
				Player *pl, float lvlWidth, int lvl);
	virtual ~TrollControl(void);

	// Called from the GameLayer
	void update(float dt);

private:
	friend class Weapon;

	static TrollControl		*singleton;

	std::vector<Troll*>		trolls;

	Pim::SpriteBatchNode	*actorSheet;
	Player					*player;
	GameLayer				*layer;
	float					levelWidth;
	int						levelNum;
};

