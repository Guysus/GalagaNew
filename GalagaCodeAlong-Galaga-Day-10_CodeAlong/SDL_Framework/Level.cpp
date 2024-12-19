#include "Level.h"

void Level::StartStage() {
	mStageStarted = true;
}

Level::Level(int stage, PlaySideBar* sideBar, Player* player) {
	mTimer = Timer::Instance();
	mSideBar = sideBar;
	mSideBar->SetLevel(stage);
	mBackgroundStars = BackgroundStars::Instance();

	mStage = stage;
	mStageStarted = false;

	mLabelTimer = 0.0f;

	mStageLabel = new Texture("STAGE", "emulogic.ttf", 32, { 75, 75, 200 });
	mStageLabel->Parent(this);
	mStageLabel->Position(Graphics::SCREEN_WIDTH * 0.35f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageNumber = new Scoreboard({ 75, 75, 200 });
	mStageNumber->Score(stage);
	mStageNumber->Parent(this);
	mStageNumber->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageLabelOnScreen = 0.0f;
	mStageLabelOffScreen = 1.5f;

	mReadyLabel = new Texture("READY", "emulogic.ttf", 32, { 150, 0, 0 });
	mReadyLabel->Parent(this);
	mReadyLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);

	mReadyLabelOnScreen = mStageLabelOffScreen;
	mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

	mPlayer = player;
	mPlayerHit = false;
	mRespawnDelay = 3.0f;
	mRespawnLabelOnScreen = 2.0f;

	mGameOverLabel = new Texture("GAME OVER!", "emulogic.ttf", 32, { 150, 0 ,0 });
	mGameOverLabel->Parent(this);
	mGameOverLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);
	
	mGameOverDelay = 6.0f;
	mGameOverTimer = 0.0f;
	mGameOverLabelOnScreen = 1.0f;

	mCurrentState = Running;

	mFormation = new Formation();
	mFormation->Position(Graphics::SCREEN_WIDTH * 0.4f, 150.0f);
	Enemy::SetFormation(mFormation);

	mButterflyCount = 0;
	mWaspCount = 0;
	mBossCount = 0;

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Data/Level1.xml");
	mSpawningPatterns.LoadFile(fullPath.c_str());

	mChallengeStage = mSpawningPatterns.FirstChildElement("Level")->FirstChildElement()->BoolAttribute("value");

	if (!mChallengeStage) {
		mFormation = new Formation();
		mFormation->Position(Graphics::SCREEN_WIDTH * 0.4f, 150.0f);
		Enemy::SetFormation(mFormation);

		for (int i = 0; i < MAX_BUTTERFLIES; i++) {
			mFormationButterflies[i] = nullptr;
		}

		for (int i = 0; i < MAX_WASPS; i++) {
			mFormationWasp[i] = nullptr;
		}

		for (int i = 0; i < MAX_BOSSES; i++) {
			mFormationBoss[i] = nullptr;
		}

		//create our formation
		//initialize our enemy arrays
	}

	mCurrentFlyInPriority = 0;
	mCurrentFlyInIdex = 0;
	mSpawnDelay = 0.2f;
	mSpawnTimer = 0;
	mSpawningFinished = false;
}

Level::~Level() {
	mTimer = nullptr;
	mSideBar = nullptr;
	mBackgroundStars = nullptr;

	delete mStageLabel;
	mStageLabel = nullptr;
	delete mStageNumber;
	mStageNumber = nullptr;
	delete mReadyLabel;
	mReadyLabel = nullptr;

	delete mGameOverLabel;
	mGameOverLabel = nullptr;

	mPlayer = nullptr;

	delete mFormation;
	mFormation = nullptr;

	for (int i = 0; i < MAX_BUTTERFLIES; i++) {
		delete mFormationButterflies[i];
		mFormationButterflies[i] = nullptr;
	}

	for (int i = 0; i < MAX_WASPS; i++) {
		delete mFormationWasp[i];
		mFormationWasp[i] = nullptr;
	}

	for (int i = 0; i < MAX_BOSSES; i++) {
		delete mFormationBoss[i];
		mFormationBoss[i] = nullptr;
	}

	for (auto enemy : mEnemies) {
		delete enemy;
		enemy = nullptr;
	}
	mEnemies.clear();
}

Level::LevelStates Level::State() {
	return mCurrentState;
}

void Level::HandleStartLabels() {
	mLabelTimer += mTimer->DeltaTime();

	if (mLabelTimer >= mStageLabelOffScreen) {
		mBackgroundStars->Scroll(true);
		mPlayer->Active(true);
		mPlayer->Visible(true);

		if (mStage > 1) {
			StartStage();
		}
		else {
			//READY label will only display at the start of level 1
			if (mLabelTimer >= mReadyLabelOffScreen) {
				StartStage();
			}
		}
	}
}

void Level::HandleCollisions() {
	if (!mPlayerHit) {
		//Quick sanity test
		if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_X)) {
			mPlayer->WasHit();
			mSideBar->SetShips(mPlayer->Lives());

			mPlayerHit = true;
			mRespawnTimer = 0.0f;
			mPlayer->Active(false);
			mBackgroundStars->Scroll(false);
		}
	}
}

void Level::HandlePlayerDeath() {
	if (!mPlayer->IsAnimating()) {
		if (mPlayer->Lives() > 0) {
			if (mRespawnTimer == 0.0f) {
				//The player has finished their death animation and we want them to respawn
				//Hide them until the respawn timer has finished
				mPlayer->Visible(false);
			}

			mRespawnTimer += mTimer->DeltaTime();
			if (mRespawnTimer >= mRespawnDelay) {
				//This is when we respawn the player
				mPlayer->Active(true);
				mPlayer->Visible(true);
				mPlayerHit = false;
				mBackgroundStars->Scroll(true);
			}
		}
		else {
			//This is GAME OVER
			if (mGameOverTimer == 0.0f) {
				mPlayer->Visible(false);
			}

			mGameOverTimer += mTimer->DeltaTime();
			if (mGameOverTimer >= mGameOverDelay) {
				mCurrentState = GameOver;
			}
		}
	}
}

void Level::HandleEnemySpawning() {
	mSpawnTimer += mTimer->DeltaTime();

	if (mSpawnTimer >= mSpawnDelay) {
		XMLElement* element = mSpawningPatterns.FirstChildElement("Level")->FirstChild()->NextSiblingElement();
		bool spawned = false;
		bool priorityFound = false;

		while (element != nullptr) {
			int priority = element->IntAttribute("priority");

			if (mCurrentFlyInPriority == priority) {
				priorityFound = true;
				int path = element->IntAttribute("path");
				XMLElement* child = element->FirstChildElement();

				//this for is always going to give us the next/last child based on our flyinIndex
				for (int i = 0; i < mCurrentFlyInIdex && child != nullptr; i++) {
					child = child->NextSiblingElement();
				}

				if (child != nullptr) {
					std::string type = child->Attribute("type");
					int index = child->IntAttribute("index");

					if (type.compare("Butterfly") == 0) {
						if (!mChallengeStage) {
							mFormationButterflies[index] = new Butterfly(path, index, false);
							mButterflyCount++;
						}
						else {//change boolean to true once challenge logic
							mEnemies.push_back(new Butterfly(path, index, false));
						}
					}
					else if (type.compare("Wasp") == 0) {
						if (!mChallengeStage) {
							mFormationWasp[index] = new Wasp(path, index, false, false);
							mWaspCount++;
						}
						else {
							mEnemies.push_back(new Wasp(path, index, false, false));
						}
					}
					else if (type.compare("Boss") == 0) {
						if (!mChallengeStage) {
							mFormationBoss[index] = new Boss(path, index, false);
							mBossCount++;
						}
						else {
							mEnemies.push_back(new Boss(path, index, false));
						}
					}

					spawned = true;
				}
			}

			element = element->NextSiblingElement();
		}

		if (!priorityFound) {
			mSpawningFinished = true;
		}
		else {
			if (!spawned) {
				//we have spawn elements waiting but we didnt spawn anything
				if (!EnemyFlyingIn()) {
					mCurrentFlyInPriority++;
					mCurrentFlyInIdex = 0;
				}
			}
			else {
				//we havent finished spawning our element enemies next index
				mCurrentFlyInIdex++;
			}
		}

		mSpawnTimer = 0;
	}

	//TODO: Remove when finished testing

	/*if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_S) &&
		mButterflyCount < MAX_BUTTERFLIES) {
		mEnemies.push_back(new Butterfly(0, mButterflyCount++, false));
	}

	if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_W) &&
		mWaspCount < MAX_WASPS) {
		mEnemies.push_back(new Wasp(0, mWaspCount++, false, false));
	}

	if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_F) &&
		mBossCount < MAX_BOSSES) {
		mEnemies.push_back(new Boss(0, mBossCount++, false));
	}*/
}

bool Level::EnemyFlyingIn() {
	for (Butterfly* butterfly : mFormationButterflies) {
		if (butterfly != nullptr && butterfly->CurrentState() == Enemy::FlyIn) {
			return true;
		}
	}

	return false;
}

void Level::HandleEnemyFormation() {
	mFormation->Update();

	/*if (mButterflyCount == MAX_BUTTERFLIES && 
		mWaspCount == MAX_WASPS &&
		mBossCount == MAX_BOSSES) {
		bool flyIn = false;
		for (auto enemy : mEnemies) {
			if (enemy->CurrentState() == Enemy::FlyIn) {
				flyIn = true;
				break;
			}
		}

		if (!flyIn) {
			mFormation->Lock();
		}
	}*/

	for (Butterfly* butterfly : mFormationButterflies)
	{
		if (butterfly != nullptr)
		{
			butterfly->Update();
		}
	}

	for (Wasp* wasp : mFormationWasp)
	{
		if (wasp != nullptr)
		{
			wasp->Update();
		}
	}

	for (Boss* boss : mFormationBoss)
	{
		if (boss != nullptr)
		{
			boss->Update();
		}
	}

	if (!mFormation->Locked())
	{
		if (mButterflyCount == MAX_BUTTERFLIES && mWaspCount == MAX_WASPS && mBossCount == MAX_BOSSES)
		{
			if (!EnemyFlyingIn())
			{
				mFormation->Lock();
			}
		}
	}
	else
	{
		HandleEnemyDiving();
	}
}

void Level::HandleEnemyDiving() { 
	if (mFormation->Locked()) {
		if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_V)) {
			for (auto enemy : mEnemies) {
				if (enemy->Type() == Enemy::Wasp && enemy->CurrentState() == Enemy::InFormation) {
					enemy->Dive();
					break;
				}
			}
		}

		if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_B)) {
			for (auto enemy : mEnemies) {
				if (enemy->Type() == Enemy::Butterfly && enemy->CurrentState() == Enemy::InFormation) {
					enemy->Dive();
					break;
				}
			}
		}

		if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_H)) {
			for (auto enemy : mEnemies) {
				if (enemy->Type() == Enemy::Boss && enemy->CurrentState() == Enemy::InFormation) {
					enemy->Dive();

					int index = enemy->Index();
					int firstEscortIndex = (index % 2 == 0) ?
						(index * 2) : (index * 2 - 1);
					int secondEscortIndex = firstEscortIndex + 4;

					for (auto butterfly : mEnemies) {
						//Verify the enemy is a butterfly in formation and the butterfly has either first or second escortindex

						//if (butterfly->Type() != Enemy::Butterfly) continue; 

						if (butterfly->Type() == Enemy::Butterfly &&
							butterfly->CurrentState() == Enemy::InFormation &&
							(butterfly->Index() == firstEscortIndex || butterfly->Index() == secondEscortIndex)) {
							butterfly->Dive(1);
						}
					}

					break;
				}
			}
		}
	}
}

void Level::Update() {
	if (!mStageStarted) {
		HandleStartLabels();
	}
	else {
		if (!mSpawningFinished) {

			HandleEnemySpawning();
		}

		if (!mChallengeStage) {

			HandleEnemyFormation();
		}
		else
		{
			for (auto enemy : mEnemies) {
				enemy->Update();
			}
		}

		HandleCollisions();

		if (mPlayerHit) {
			HandlePlayerDeath();
		}
		else {
			//TODO: Temporary logic until enemeies implemented
			if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_N)) {
				mCurrentState = Finished;
			}
		}
	}
}

void Level::Render() {
	if (!mStageStarted) {
		if (mLabelTimer > mStageLabelOnScreen && mLabelTimer < mStageLabelOffScreen) {
			mStageLabel->Render();
			mStageNumber->Render();
		}
		else if (mLabelTimer > mReadyLabelOnScreen && mLabelTimer < mReadyLabelOffScreen) {
			mReadyLabel->Render();
		}
	}
	else {
		/*for (auto enemy : mEnemies) {
			enemy->Render();
		}

		if (mPlayerHit) {
			if (mRespawnTimer >= mRespawnLabelOnScreen) {
				mReadyLabel->Render();
			}

			if (mGameOverTimer >= mGameOverLabelOnScreen) {
				mGameOverLabel->Render();
			}
		}*/

		if (!mChallengeStage)
		{
			for (Butterfly* butterfly : mFormationButterflies)
			{
				if (butterfly != nullptr)
				{
					butterfly->Render();
				}
			}

			for (Wasp* wasp : mFormationWasp)
			{
				if (wasp != nullptr)
				{
					wasp->Render();
				}
			}

			for (Boss* boss : mFormationBoss)
			{
				if (boss != nullptr)
				{
					boss->Render();
				}
			}
		}
		else
		{
			for (auto enemy : mEnemies)
			{
				enemy->Render();
			}
		}
	}
}