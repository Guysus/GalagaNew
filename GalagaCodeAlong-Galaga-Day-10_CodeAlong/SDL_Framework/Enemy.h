#pragma once
#include "BezierPath.h"
#include "AnimatedTexture.h"
#include "PhysEntity.h"
#include "Player.h"
#include "Formation.h"

using namespace SDLFramework;

class Enemy : public PhysEntity {
public:
	//If we add new states here, make sure to update HandleStates in the .cpp
	enum States { FlyIn, InFormation, Diving, Dead };
	enum Types { Butterfly, Wasp, Boss };

	static void CreatePaths();
	static void SetFormation(Formation* formation);
	static void CurrentPlayer(Player* player);

	States CurrentState();
	Types Type();
	int Index();
	bool InDeathAnimation();
	int GetKillCount();

	Enemy(int path, int index, bool challenge);
	virtual ~Enemy();

	virtual void Dive(int type = 0);
	virtual void Hit(PhysEntity* other) override;

	void Update() override;
	void Render() override;

protected:
	static Formation* sFormation;
	static std::vector<std::vector<Vector2>> sPaths;
	static Player* sPlayer;

	Timer* mTimer;

	Texture* mTextures[2];
	AnimatedTexture* mDeathAnimation;

	States mCurrentState;
	Types mType;

	int mIndex;
	int mKillCount;

	bool mChallengeStage;

	Vector2 mDiveStartPosition;

	unsigned mCurrentPath;

	unsigned mCurrentWaypoint;
	const float EPSILON = 50.0f;

	float mSpeed;

	bool IgnoreCollisions() override;

	virtual void PathComplete();
	virtual void FlyInComplete();

	void JoinFormation();

	virtual Vector2 WorldFormationPosition();
	virtual Vector2 LocalFormationPosition() = 0;

	virtual void HandleFlyInState();
	virtual void HandleInFormationState();
	virtual void HandleDiveState() = 0;
	virtual void HandleDeadState();

	void HandleStates();

	virtual void RenderFlyInState();
	virtual void RenderInFormationState();
	virtual void RenderDiveState() = 0;
	virtual void RenderDeadState();

	void RenderStates();
};