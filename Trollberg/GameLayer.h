#pragma once
#include "Trollberg.h"

// Forward declarations
class Rock;
class Player;
class TrollControl;


class GameLayer : public Pim::Layer
{
public:
	static GameLayer* getSingleton();
	static Pim::SpriteBatchNode* getActorSheet();
	static bool isPlayerDead();

	GameLayer(int lvl);
	virtual ~GameLayer();

	void draw();

	void loadResources();
	void loadLightingSystem();	// Create light sys, reload textures used by the guns
	void loadRain();
	void cacheSounds();

	// Provide the game layer with a batch node (containing the texture), and a 
	// rectangle which will be rendered from said aa.
	void setSpriteInformation(Pim::SpriteBatchNode *b, Pim::Rect r);

	// Create a b2Body based on the vertex information in the Polygons-struct.
	void createGroundBody(Polygons &poly);
	void createLevelEdges();

	void update(float dt);
	void updateRain(float dt);
	void followPlayer();

	void keyEvent(Pim::KeyEvent &evt);

private:
	static GameLayer			*singleton;

	// The troll spawn controller
	TrollControl				*trollControl;

	// The ground sprite. 
	Pim::Sprite					*ground;

	// The rain droplets
	std::vector<Pim::Sprite*>	rain;

	// The Player object. Playaaaa!
	Player						*player;

	// The actor sheet. 
	Pim::SpriteBatchNode		*actorSheet;

	// The Box2D world. The GameScene is the owner of the world, this class
	// is only keeping a reference to it.
	b2World						*world;

	// The body of the ground, created via the "createGroundBody(...)" function.
	b2Body						*groundBody;

	// The width of the level in pixels
	float						levelWidth;

	// The number of the current level (1-3)
	int							levelNum;
};

