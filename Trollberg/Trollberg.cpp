#include "Pim.h"
#include "GameScene.h"

int main()
{
	{   // Stack cheating: avoid 'cd' from "leaking" memory
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
		gc->go(new GameScene(1), cd);
		delete gc;

		// Test
	}

#ifdef _DEBUG
	// Prints leaked memory TO THE OUTPUT TAB. Not the console. The outputtab in VS. 
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}


// Pim::Vec2 to b2Vec2 conversion. PTM-Ratio is taken into account.
b2Vec2 toB2(const Pim::Vec2 &v)
{
	return b2Vec2(v.x / PTMR, v.y / PTMR);
}

// b2Vec2 to Pim::Vec2 conversion. PTM-Ratio is taken into account.
Pim::Vec2 toPim(const b2Vec2 &v)
{
	return Pim::Vec2(v.x * PTMR, v.y * PTMR);
}


// Something went horribly wrong. Display an error message and exit the game completely.
void TrollbergExit(std::string s)
{
	MessageBoxA(NULL, s.c_str(), "Trololol", MB_OK | MB_ICONEXCLAMATION);
	exit(1);
}