#include "Pim.h"

int main()
{
	Pim::WinStyle::CreationData cd;

	cd.resolution					= Pim::Vec2(960, 540);
	cd.defaultWindowedResolution	= cd.resolution;

	cd.renderResolution				= Pim::Vec2(384, 216);
	cd.coordinateSystem				= cd.renderResolution;

	cd.forcedAspectRatio			= true;
	cd.aspectRatio					= 16.f / 9.f;

	cd.winTitle						= "Trollberg";
	cd.winStyle						= Pim::WinStyle::WINDOWED;

	Pim::GameControl *gc = new Pim::GameControl;
	gc->go(new Pim::Scene, cd);
	delete gc;

	return 0;
}