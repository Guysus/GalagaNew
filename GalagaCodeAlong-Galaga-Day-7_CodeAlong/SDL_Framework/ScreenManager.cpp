#include "ScreenManager.h"

ScreenManager* ScreenManager::sInstance = nullptr;

ScreenManager* ScreenManager::Instance() {
	if (sInstance == nullptr) {
		sInstance = new ScreenManager();
	}

	return sInstance;
}

void ScreenManager::Release() {
	delete sInstance;
	sInstance = nullptr;
}

ScreenManager::ScreenManager() {
	mBackgroundStars = BackgroundStars::Instance();
	mInput = InputManager::Instance();

	//Screens
	mStartScreen = new StartScreen();
	mPlayScreen = new PlayScreen();

	mCurrentScreen = Start;
}

ScreenManager::~ScreenManager() {
	mInput = nullptr;

	BackgroundStars::Release();
	mBackgroundStars = nullptr;

	delete mStartScreen;
	mStartScreen = nullptr;

	delete mPlayScreen;
	mPlayScreen = nullptr;
}

void ScreenManager::Update() {
	mBackgroundStars->Update();

	switch (mCurrentScreen)
	{
	case ScreenManager::Start:
		mStartScreen->Update();

		if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
			//We want to switch from the start screen to the play screen
			mCurrentScreen = Play;
			mStartScreen->ResetAnimation();
			mPlayScreen->StartNewGame();
		}
		break;

	case ScreenManager::Play:
		mPlayScreen->Update();

		if (mPlayScreen->GameOver()) {
			mCurrentScreen = Start;
		}

		break;

	default:
		std::cerr << "Unknown state found! Please make sure to assign a valid Game Screen." << std::endl;
		break;
	}
}

void ScreenManager::Render() {
	mBackgroundStars->Render();

	switch (mCurrentScreen)
	{
	case ScreenManager::Start:
		mStartScreen->Render();
		break;

	case ScreenManager::Play:
		mPlayScreen->Render();
		break;
	}
}