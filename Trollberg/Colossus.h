#pragma once
#include "troll.h"

// Forward declarations
class ColossusAI;
class Player;

class Colossus : public Troll
{
public:
	Colossus(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);
	~Colossus();

	void createPhysics();
	void createLight();

	void createCrushSensor();
	void destroyCrushSensor();

	void update(float dt);

	void deleteBody();

private:
	friend class ColossusAI;

	b2Body					*body2;
	b2Body					*crushSensor;

	Pim::PreloadLightDef	*pld;

	Animation				walkAnim;
	Animation				crushAnim;
	Animation				deathAnim;

	float					deathTimer;
	bool					dead;
};

