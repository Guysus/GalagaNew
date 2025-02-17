#pragma once
#include "Enemy.h"

class Butterfly : public Enemy {
public:
	static void CreateDivePaths();

	void Dive(int type = 0) override;
	void Hit(PhysEntity* other) override;

	Butterfly(int path, int index, bool challenge);
	~Butterfly();

private:
	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mEscort;

	Vector2 LocalFormationPosition() override;

	void HandleDiveState() override;

	void RenderDiveState() override;
};