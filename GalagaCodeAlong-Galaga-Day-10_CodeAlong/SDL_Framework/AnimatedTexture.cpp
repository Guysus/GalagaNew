#include "AnimatedTexture.h"

namespace SDLFramework {
	void AnimatedTexture::RunAnimation() {
		if (!mAnimationDone) {
			mAnimationTimer += mTimer->DeltaTime();

			if (mAnimationTimer >= mAnimationSpeed) {
				//This means we have finished one iteration of our animation
				if (mWrapMode == Loop) {
					//reset timer, accounting for any extra time
					mAnimationTimer -= mAnimationSpeed;
				}
				else {
					mAnimationDone = true;
					//back the timer up to the last frame of the animation
					mAnimationTimer = mAnimationSpeed - mTimePerFrame;
				}

			}

			if (mAnimationDirection == Horizontal) {
				mSourceRect.x = mStartx + (int)(mAnimationTimer / mTimePerFrame) * mWidth;
			}
			else {
				mSourceRect.y = mStarty + (int)(mAnimationTimer / mTimePerFrame) * mHeight;
			}
		}
	}

	AnimatedTexture::AnimatedTexture(std::string filename, int x, int y, int width, int height,
						int frameCount, float animationSpeed, AnimDir animationDir, bool managed) :
						Texture(filename, x, y, width, height, managed) {
		mTimer = Timer::Instance();

		mStartx = x;
		mStarty = y;

		mFrameCount = frameCount;
		mAnimationSpeed = animationSpeed;
		mTimePerFrame = mAnimationSpeed / mFrameCount;
		mAnimationTimer = 0.0f;

		mWrapMode = Loop;
		mAnimationDirection = animationDir;

		mAnimationDone = false;
	}

	AnimatedTexture::~AnimatedTexture() {
		mTimer = nullptr;
	}

	void AnimatedTexture::SetWrapMode(WrapMode wrapMode) {
		mWrapMode = wrapMode;
	}

	bool AnimatedTexture::IsAnimating() {
		return !mAnimationDone;
	}

	void AnimatedTexture::ResetAnimation() {
		mAnimationTimer = 0.0f;
		mAnimationDone = false;
	}

	void AnimatedTexture::Update() {
		RunAnimation();
	}
}