#pragma once
#include "Enemy.h"

class Wasp : public Enemy {
public:
	static void CreateDivePaths();

	Wasp(int path, int index, bool challenge, bool diver);
	~Wasp();
private:
	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mDiver;

	void FlyInComplete() override;

	Vector2 LocalFormationPosition() override;

	void HandleDiveState() override;
	void HandleDeadState() override;

	void RenderDiveState() override;
	void RenderDeadState() override;
};