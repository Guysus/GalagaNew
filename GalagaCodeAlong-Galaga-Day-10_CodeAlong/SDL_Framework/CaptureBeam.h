#pragma once
#include "AnimatedTexture.h"
#include "PhysEntity.h"
#include "BoxCollider.h"

using namespace SDLFramework;

class CaptureBeam : public AnimatedTexture
{
private:

	void RunAnimation() override;
	bool IgnoreCollision();

public:
	CaptureBeam();
	~CaptureBeam();

	void ResetAnimation() override;
	void Render() override;

private:
	float mTotalCaptureTime;
	float mCaptureTimer;
};