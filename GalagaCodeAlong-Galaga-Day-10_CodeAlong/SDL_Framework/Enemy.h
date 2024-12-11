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

	States CurrentState();
	Types Type();
	int Index();

	Enemy(int path, int index, bool challenge);
	virtual ~Enemy();

	virtual void Dive(int type = 0);

	void Update() override;
	void Render() override;

protected:
	static Formation* sFormation;
	static std::vector<std::vector<Vector2>> sPaths;
	static Player* sPlayer;

	Timer* mTimer;

	Texture* mTextures[2];

	States mCurrentState;
	Types mType;

	int mIndex;

	bool mChallengeStage;

	Vector2 mDiveStartPosition;

	unsigned mCurrentPath;

	unsigned mCurrentWaypoint;
	const float EPSILON = 50.0f;

	float mSpeed;

	virtual void PathComplete();
	virtual void FlyInComplete();

	void JoinFormation();

	virtual Vector2 WorldFormationPosition();
	virtual Vector2 LocalFormationPosition() = 0;

	virtual void HandleFlyInState();
	virtual void HandleInFormationState();
	virtual void HandleDiveState() = 0;
	virtual void HandleDeadState() = 0;

	void HandleStates();

	virtual void RenderFlyInState();
	virtual void RenderInFormationState();
	virtual void RenderDiveState() = 0;
	virtual void RenderDeadState() = 0;

	void RenderStates();
};