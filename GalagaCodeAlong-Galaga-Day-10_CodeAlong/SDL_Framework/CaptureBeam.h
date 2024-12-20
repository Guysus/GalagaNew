#pragma once
#include "AnimatedTexture.h"

using namespace SDLFramework;

class CaptureBeam : public AnimatedTexture
{
private:

	void RunAnimation() override;

public:
	CaptureBeam();
	~CaptureBeam();

	void ResetAnimation() override;
	void Render() override;

private:
	float mTotalCaptureTime;
	float mCaptureTimer;
};