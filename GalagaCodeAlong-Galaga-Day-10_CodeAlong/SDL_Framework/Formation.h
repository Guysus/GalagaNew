#pragma once
#include "GameEntity.h"
#include "Timer.h"

using namespace SDLFramework;

class Formation : public GameEntity {
public:
	Formation();
	~Formation();

	void Update() override;

	int GetTick();

	Vector2 GridSize();

	bool Locked();
	void Lock();

private:
	Timer* mTimer;

	//Side-to-side movement while enemies are still spawning
	float mOffsetAmount;
	float mOffsetTimer;
	float mOffsetDelay;
	int mOffsetCounter;
	int mOffsetDirection;

	//Pulsing animation when formation is locked
	float  mSpreadTimer;
	float mSpreadDelay;
	int mSpreadCounter;
	int mSpreadDirection;

	Vector2 mGridSize;

	bool mLocked;
};