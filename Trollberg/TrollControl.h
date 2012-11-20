#pragma once

// Forward declarations
class GameLayer;
class Player;
class Troll;

class TrollControl
{
public:
	TrollControl(Pim::SpriteBatchNode *batch, GameLayer *layer, 
				Player *pl, float lvlWidth, int lvl);
	virtual ~TrollControl(void);

	// Called from the GameLayer
	void update(float dt);

private:
	std::vector<Troll*>		trolls;

	Pim::SpriteBatchNode	*actorSheet;
	Player					*player;
	GameLayer				*layer;
	float					levelWidth;
	int						levelNum;
};

