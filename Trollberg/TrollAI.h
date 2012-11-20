#pragma once

// Forward declarations
class Troll;

class TrollAI
{
public:
	virtual ~TrollAI();

private:
	TrollAI(){}
	TrollAI(const TrollAI&) {}

	Troll *troll;
};

