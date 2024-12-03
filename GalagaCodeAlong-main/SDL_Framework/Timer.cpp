#include "Timer.h"

namespace SDLFramework {
	Timer* Timer::sInstance = nullptr;

	Timer* Timer::Instance() {
		if (sInstance == nullptr) {
			sInstance = new Timer();
		}

		return sInstance;
	}

	void Timer::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	void Timer::Reset() {
		mStartTicks = SDL_GetTicks();
	}

	float Timer::DeltaTime() {
		return mDeltaTime;
	}

	void Timer::TimeScale(float ts) {
		mTimeScale = ts;
	}

	float Timer::TimeScale() {
		return mTimeScale;
	}

	void Timer::Update() {
		//Calculating the difference in time (in milliseconds)
		//Between the start of the current frame and the end of the current frame
		mElapsedTicks = SDL_GetTicks() - mStartTicks;
		mDeltaTime = mElapsedTicks * 0.001f;
	}

	Timer::Timer() {
		Reset();
		mElapsedTicks = 0;
		mDeltaTime = 0.0f;
		mTimeScale = 1.0f; //Running at normal speed
	}

	Timer::~Timer() {

	}
}