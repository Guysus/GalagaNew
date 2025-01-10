#pragma once
#include "PlaySideBar.h"
#include "BackgroundStars.h"
#include "Player.h"
#include "Butterfly.h"
#include "Wasp.h"
#include "Boss.h"
#include "tinyxml2.h"

using namespace SDLFramework;
using namespace tinyxml2;

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

	static const int MAX_BUTTERFLIES = 16;
	static const int MAX_WASPS = 20;
	static const int MAX_BOSSES = 4;
	static const int MAX_ENEMIES = 40;

	int mButterflyCount;
	int mWaspCount;
	int mBossCount;
	int mEnemiesKilled;
	std::vector<Enemy*> mEnemies;
	
	Butterfly* mFormationButterflies[MAX_BUTTERFLIES];
	Wasp* mFormationWasp[MAX_WASPS];
	Boss* mFormationBoss[MAX_BOSSES];

	XMLDocument mSpawningPatterns;
	int mCurrentFlyInPriority;
	int mCurrentFlyInIdex;

	float mSpawnDelay;
	float mSpawnTimer;
	bool mSpawningFinished;

	int mStage;
	bool mChallengeStage;
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
	bool mButterflyHit;
	bool mWaspHit;
	bool mBossHit;
	float mRespawnDelay;
	float mRespawnTimer;
	float mRespawnLabelOnScreen;

	Texture* mGameOverLabel;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	Butterfly* mDivingButterfly;
	bool mSkipFirstButterfly;
	float mButterflyDiveDelay;
	float mButterflyDiveTimer;

	Wasp* mDivingWasp;
	Wasp* mDivingWasp2;
	float mWaspDiveDelay;
	float mWaspDiveTimer;

	Boss* mDivingBoss;
	bool mCaptureDive;
	bool mSkipFirstBoss;
	float mBossDiveDelay;
	float mBossDiveTimer;

	LevelStates mCurrentState;

	void HandleStartLabels();
	void HandleCollisions();
	void HandlePlayerDeath();

	void StartStage();

	bool EnemyFlyingIn();

	void HandleEnemyDeath();
	void HandleEnemySpawning();
	void HandleEnemyFormation();
	void HandleEnemyDiving();
};