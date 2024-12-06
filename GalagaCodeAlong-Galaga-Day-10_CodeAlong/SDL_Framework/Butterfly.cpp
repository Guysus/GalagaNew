#include "Butterfly.h"

Vector2 Butterfly::LocalFormationPosition() {
	Vector2 retVal;

	int dir = mIndex % 2 == 0 ? -1 : 1;

	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 4)) * (float)dir;
	retVal.y = sFormation->GridSize().y * ((mIndex % 4) / 2);

	return retVal;
}

void Butterfly::HandleDiveState() { }
void Butterfly::HandleDeadState() { }

void Butterfly::RenderDiveState() { }
void Butterfly::RenderDeadState() { }

Butterfly::Butterfly(int path, int index, bool challenge) :
Enemy(path, index, challenge)
{ 
	mTexture = new Texture("AnimatedEnemies.png", 0, 0, 52, 40);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mType = Enemy::Butterfly;
}

Butterfly::~Butterfly() { }