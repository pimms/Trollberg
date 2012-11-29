#pragma once

#include <iostream>

class OLLeaderboard
{
public:
	OLLeaderboard();
	~OLLeaderboard();

	bool connect();

private:
	// The singleton is inaccessible from outside classes, but is used for the 
	// static libCurl callback functions
	static OLLeaderboard* singleton;
};

