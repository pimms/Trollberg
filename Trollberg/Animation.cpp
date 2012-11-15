#include "Animation.h"


Animation::Animation(Pim::Vec2 firstFrameXY, int frameW, int frameH, int totalFrame, int horFrames)
{
	firstFramePos		= firstFrameXY;
	frameWidth			= frameW;
	frameHeight			= frameH;
	totalFrames			= totalFrame;
	framesInAnimation   = totalFrames;
	horizontalFrames	= horFrames;

	curFrame	= 0;
	frameTime	= 0.1f;
	curTime		= 0.f;
}
Animation::Animation()
{
	frameWidth			= 0;
	frameHeight			= 0;
	totalFrames			= 0;
	framesInAnimation	= 0;
	horizontalFrames	= 0;

	curFrame	= 0;
	frameTime	= 0.1f;
	curTime		= 0.f;
}

Pim::Rect Animation::update(float dt)
{
	curTime += dt;
	if (curTime >= frameTime)
	{
		curTime -= frameTime;
		return nextFrame();
	}

	return frameIndex(curFrame);
}
Pim::Rect Animation::reset()
{
	curTime = 0.f;
	return frameIndex(0);
}

Pim::Rect Animation::nextFrame()
{
	curFrame++;
	if (curFrame >= framesInAnimation)
		curFrame = 0;

	return frameIndex(curFrame);
}
Pim::Rect Animation::prevFrame()
{
	curFrame--;
	if (curFrame < 0)
		curFrame = framesInAnimation-1;

	return frameIndex(curFrame);
}
Pim::Rect Animation::frameIndex(int idx)
{
	curFrame = idx;

	Pim::Rect r;
	r.x = firstFramePos.x + frameWidth*(curFrame % horizontalFrames);
	r.y = firstFramePos.y + frameHeight*(curFrame / horizontalFrames);
	r.width = frameWidth;
	r.height = frameHeight;

	return r;
}