#include "VersionControl.h"


VersionControl* VersionControl::singleton = NULL;

void VersionControl::instantiateSingleton()
{
	if (!singleton)
	{
		singleton = new VersionControl;
	}
}
void VersionControl::clearSingleton()
{
	if (singleton)
	{
		delete singleton;
		singleton = NULL;
	}
}
float VersionControl::getGLVersion()
{
	return singleton->glVersion;
}

VersionControl::VersionControl(void)
{
 	const GLubyte *string = glGetString(GL_VERSION);

	glVersion = atof((const char*)string);

	printf("GL VERSION: %f", glVersion);
}
