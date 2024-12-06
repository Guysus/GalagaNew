#pragma once
#include "Enemy.h"

class Butterfly : public Enemy {
public:
	Butterfly(int path, int index, bool challenge);
	~Butterfly();

private:
	Vector2 LocalFormationPosition() override;

	void HandleDiveState() override;
	void HandleDeadState() override;

	void RenderDiveState() override;
	void RenderDeadState() override;
};