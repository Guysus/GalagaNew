#include "PlaySideBar.h"

PlaySideBar::PlaySideBar() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mBackground = new Texture("Black.png");
	mBackground->Parent(this);
	mBackground->Scale(Vector2(3.0f, 10.0f));
	mBackground->Position(45.0f, 380.0f);

	mHighLabel = new Texture("HIGH", "emulogic.ttf", 32, { 150, 0, 0 });
	mHighLabel->Parent(this);
	mHighLabel->Position(-25.0f, 0.0f);

	mScoreLabel = new Texture("SCORE", "emulogic.ttf", 32, { 150, 0, 0 });
	mScoreLabel->Parent(this);
	mScoreLabel->Position(25.0f, 32.0f);

	mHighScoreBoard = new Scoreboard();
	mHighScoreBoard->Parent(this);
	mHighScoreBoard->Position(90.0f, 64.0f);

	mOneUpLabel = new Texture("1UP", "emulogic.ttf", 32, { 150, 0, 0 });
	mOneUpLabel->Parent(this);
	mOneUpLabel->Position(-45.0f, 160.0f);

	mBlinkTimer = 0.0f;
	mBlinkInterval = 0.5f;
	mOneUpLabelVisible = true;

	mPlayerOneScore = new Scoreboard();
	mPlayerOneScore->Parent(this);
	mPlayerOneScore->Position(90.0f, 192.0f);

	mShips = new GameEntity();
	mShips->Parent(this);
	mShips->Position(-40.0f, 420.0f);

	for (int i = 0; i < MAX_SHIP_TEXTURES; i++) {
		mShipTextures[i] = new Texture("PlayerShips.png", 0, 0, 60, 64);
		mShipTextures[i]->Parent(mShips);
		mShipTextures[i]->Position(62.0f * (i % 3), 70.0f * (i / 3));
	}

	mTotalShipsLabel = new Scoreboard();
	mTotalShipsLabel->Parent(mShips);
	mTotalShipsLabel->Position(140.0f, 80.0f);

	mFlags = new GameEntity();
	mFlags->Parent(this);
	mFlags->Position(-50.0f, 600.0f);

	mFlagTimer = 0.0f;
	mFlagInterval = 0.25f;
}

PlaySideBar::~PlaySideBar() {
	mTimer = nullptr;
	mAudio = nullptr;

	delete mBackground;
	mBackground = nullptr;

	delete mHighLabel;
	mHighLabel = nullptr;

	delete mScoreLabel;
	mScoreLabel = nullptr;

	delete mHighScoreBoard;
	mHighScoreBoard = nullptr;

	delete mOneUpLabel;
	mOneUpLabel = nullptr;

	delete mPlayerOneScore;
	mPlayerOneScore = nullptr;

	delete mShips;
	mShips = nullptr;

	delete mTotalShipsLabel;
	mTotalShipsLabel = nullptr;

	for (auto texture : mShipTextures) {
		delete texture;
		texture = nullptr;
	}

	delete mFlags;
	mFlags = nullptr;

	ClearFlags();
}

void PlaySideBar::ClearFlags() {
	for (int i = 0; i < mFlagTextures.size(); i++) {
		delete mFlagTextures[i];
		mFlagTextures[i] = nullptr;
	}

	mFlagTextures.clear();
}

void PlaySideBar::AddNextFlag() {
	if (mRemainingLevels >= 50) {
		AddFlag("LevelFlags.png", 62, 50);
	}
	else if (mRemainingLevels >= 30) {
		AddFlag("LevelFlags.png", 62, 30);
	}
	else if (mRemainingLevels >= 20) {
		AddFlag("LevelFlags.png", 62, 20);
	}
	else if (mRemainingLevels >= 10) {
		AddFlag("LevelFlags.png", 54, 10);
	}
	else if (mRemainingLevels >= 5) {
		AddFlag("LevelFlags.png", 30, 5);
	} 
	else {
		AddFlag("LevelFlags.png", 30, 1);
	}
}

void PlaySideBar::AddFlag(std::string filename, float width, int value) {
	//.size() is giving us how MANY objects exist in our vector
	//The size is ALWAYS going to be 1 more than the last element's index number
	int index = (int)mFlagTextures.size();

	if (index > 0) {
		mFlagXOffset += width * 0.5f;
	}

	if (mFlagXOffset > 140) {
		mFlagYOffset += 66;
		mFlagXOffset = 0;
	}

	mRemainingLevels -= value;
	//The x position of the flag we want to cut from the texture
	int x = 0;

	switch (value) {
	case 50:
		x = 228;
		break;
	case 30:
		x = 168;
		break;
	case 20:
		x = 108;
		break;
	case 10:
		x = 56;
		break;
	case 5:
		x = 28;
		break;
	default:
		std::cerr << "Unknown flag given at value: " << value << ". Please enter a valid flag value." << std::endl;
	}

	mFlagTextures.push_back(new Texture(filename, x, 0, width - 2, 64));
	mFlagTextures[index]->Parent(mFlags);
	mFlagTextures[index]->Position(mFlagXOffset, mFlagYOffset);
	mFlagXOffset += width * 0.5f;

	mAudio->PlaySFX("SFX/FlagSound.wav", 0, -1);
}

void PlaySideBar::SetShips(int ships) {
	mTotalShips = ships;

	if (ships > MAX_SHIP_TEXTURES) {
		mTotalShipsLabel->Score(ships);
	}
}

void PlaySideBar::SetHighScore(int score) {
	mHighScoreBoard->Score(score);
}

void PlaySideBar::SetPlayerScore(int score) {
	mPlayerOneScore->Score(score);
}

void PlaySideBar::SetLevel(int level) {
	ClearFlags();

	mRemainingLevels = level;
	mFlagXOffset = 0.0f;
}

void PlaySideBar::Update() {
	mBlinkTimer += mTimer->DeltaTime();

	if (mBlinkTimer >= mBlinkInterval) {
		mOneUpLabelVisible = !mOneUpLabelVisible;
		mBlinkTimer = 0.0f;
	}

	if (mRemainingLevels > 0) {
		mFlagTimer += mTimer->DeltaTime();

		if (mFlagTimer >= mFlagInterval) {
			AddNextFlag();
			mFlagTimer = 0.0f;
		}
	}
}

void PlaySideBar::Render() {
	mBackground->Render();

	mHighLabel->Render();
	mScoreLabel->Render();
	mHighScoreBoard->Render();

	if (mOneUpLabelVisible) {
		mOneUpLabel->Render();
	}

	mPlayerOneScore->Render();

	for (int i = 0; i < MAX_SHIP_TEXTURES && i < mTotalShips; i++) {
		mShipTextures[i]->Render();
	}

	if (mTotalShips > MAX_SHIP_TEXTURES) {
		mTotalShipsLabel->Render();
	}

	for (auto flag : mFlagTextures) {
		flag->Render();
	}
}