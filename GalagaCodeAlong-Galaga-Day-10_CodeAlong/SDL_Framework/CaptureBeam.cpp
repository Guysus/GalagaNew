#include "CaptureBeam.h"

void CaptureBeam::RunAnimation()
{
	mCaptureTimer += mTimer->DeltaTime();
	if (mCaptureTimer >= mTotalCaptureTime)
	{
		mAnimationDone = true;
	}
	else
	{
		mAnimationTimer += mTimer->DeltaTime();
		if (mAnimationTimer >= mAnimationSpeed)
		{
			mAnimationTimer -= mAnimationSpeed;
		}

		mSourceRect.x = (int)(mAnimationTimer / mTimePerFrame) * mWidth;

		if (mCaptureTimer < 2.0f)
		{
			int temp = (int)(mCaptureTimer * 3.5f);
			mSourceRect.h = (int)(temp / 7.0f * mHeight);
		}
		else if (mCaptureTimer > mTotalCaptureTime - 2.0f)
		{
			int temp = (int)((mTotalCaptureTime - mCaptureTimer) * 3.5f);
			mSourceRect.h = (int)(temp / 7.0f * mHeight);
		}
		else
		{
			mSourceRect.h = mHeight;
		}
	}
}

CaptureBeam::CaptureBeam() : AnimatedTexture("CaptureBeam.png", 0, 0, 184, 320, 3, 0.5f, Horizontal)
{
	mTotalCaptureTime = 6.0f;
	ResetAnimation();
}

CaptureBeam::~CaptureBeam() { }

void CaptureBeam::ResetAnimation()
{
	AnimatedTexture::ResetAnimation();
	mCaptureTimer = 0.0f;
	mSourceRect.h = 0;
}

void CaptureBeam::Render()
{
	Vector2 pos = Position(World);
	Vector2 scale = Scale(World);
	mDestinationRect.x = (int)(pos.x - mWidth * scale.x * 0.5f);
	mDestinationRect.y = (int)(pos.y - mHeight * scale.y * 0.5f);
	mDestinationRect.w = (int)(mWidth * scale.x);
	mDestinationRect.h = mSourceRect.h;

	mGraphics->DrawTexture(mTex, mClipped ? &mSourceRect : nullptr, &mDestinationRect, Rotation(World));
}

