#pragma once
#include "trollai.h"

// Forward declarations
class Tumbler;

class TumblerAI : public TrollAI
{
public:
	TumblerAI(Tumbler *troll, Player *pl);
	~TumblerAI(void);

	void update(float dt);

private:
	Tumbler		*tumbler;
};

