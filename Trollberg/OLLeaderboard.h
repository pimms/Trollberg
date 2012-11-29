#pragma once

#include <iostream>
#include "curl/curl.h"

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

	CURL	*curl;
};

