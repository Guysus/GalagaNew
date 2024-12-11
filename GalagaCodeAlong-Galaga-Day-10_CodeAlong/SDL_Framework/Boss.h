#pragma once
#include "Enemy.h"

class Boss : public Enemy {
public:
	Boss(int path, int index, bool challenge);
	~Boss();

private:
	Vector2 LocalFormationPosition() override;

	void HandleDiveState() override;
	void HandleDeadState() override;

	void RenderDiveState() override;
	void RenderDeadState() override;
};