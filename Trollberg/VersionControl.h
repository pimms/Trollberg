#pragma once

#include "Pim.h"

class VersionControl
{
public:
	static void instantiateSingleton();
	static void clearSingleton();
	static float getGLVersion();

	VersionControl();

private:
	static VersionControl *singleton;
	
	float	glVersion;
};

