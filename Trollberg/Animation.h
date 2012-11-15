#pragma once

#include "Pim.h"

class Animation
{
public:
	Animation(Pim::Vec2 firstFrameXY, int frameW, int frameH, int totalFrame, int horFrmes);
	Animation();

	Pim::Rect update(float dt);
	Pim::Rect reset();

	Pim::Rect nextFrame();
	Pim::Rect prevFrame();
	Pim::Rect frameIndex(int idx);

	Pim::Vec2 firstFramePos;
	int frameWidth;
	int frameHeight;
	int totalFrames;
	int framesInAnimation;
	int horizontalFrames;
	
	int curFrame;
	float frameTime, curTime;
};

