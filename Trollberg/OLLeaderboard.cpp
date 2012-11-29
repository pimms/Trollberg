#include "OLLeaderboard.h"


OLLeaderboard* OLLeaderboard::singleton = NULL;


OLLeaderboard::OLLeaderboard(void)
{
	singleton = this;
}
OLLeaderboard::~OLLeaderboard(void)
{
	singleton = NULL;
}

bool OLLeaderboard::connect()
{
	return true;
}