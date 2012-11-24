#pragma once
#include "troll.h"

// Forward declarations
class ColossusAI;
class Player;

class Colossus : public Troll
{
public:
	Colossus(Player *pl, Pim::SpriteBatchNode *b, Pim::Vec2 p);

	void createPhysics();
	void createLight();

	void createCrushSensor();
	void destroyCrushSensor();

	void deleteBody();

private:
	friend class ColossusAI;

	b2Body					*crushSensor;

	Pim::PreloadLightDef	*pld;
};

