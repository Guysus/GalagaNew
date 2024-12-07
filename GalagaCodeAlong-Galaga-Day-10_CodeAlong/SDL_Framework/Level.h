#pragma once
#include "PlaySideBar.h"
#include "BackgroundStars.h"
#include "Player.h"
#include "Butterfly.h"
#include "Wasp.h"

using namespace SDLFramework;

class Level : public GameEntity {
public:
	enum LevelStates {Running, Finished, GameOver};

	Level(int stage, PlaySideBar* sideBar, Player* player);
	~Level();

	LevelStates State();

	void Update();
	void Render();

private:
	Timer* mTimer;
	PlaySideBar* mSideBar;
	BackgroundStars* mBackgroundStars;

	Player* mPlayer;
	Formation* mFormation;

	const int MAX_BUTTERFLIES = 16;
	const int MAX_WASPS = 20;

	int mButterflyCount;
	int mWaspCount;
	std::vector<Enemy*> mEnemies;

	int mStage;
	bool mStageStarted;

	Texture* mReadyLabel;
	float mReadyLabelOnScreen;
	float mReadyLabelOffScreen;

	Texture* mStageLabel;
	Scoreboard* mStageNumber;
	float mStageLabelOnScreen;
	float mStageLabelOffScreen;

	float mLabelTimer;

	bool mPlayerHit;
	float mRespawnDelay;
	float mRespawnTimer;
	float mRespawnLabelOnScreen;

	Texture* mGameOverLabel;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	LevelStates mCurrentState;

	void HandleStartLabels();
	void HandleCollisions();
	void HandlePlayerDeath();

	void StartStage();

	void HandleEnemySpawning();
	void HandleEnemyFormation();
	void HandleEnemyDiving();
};