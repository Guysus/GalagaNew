#include "Boss.h"

Vector2 Boss::LocalFormationPosition() {
	Vector2 retVal;
	int dir = mIndex % 2 == 0 ? -1 : 1;

	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 2)) * (float)dir;
	retVal.y = -sFormation->GridSize().y;

	return retVal;
}

void Boss::HandleDiveState() { }
void Boss::HandleDeadState() { }

void Boss::RenderDiveState() { }
void Boss::RenderDeadState() { }

Boss::Boss(int path, int index, bool challenge) :
	Enemy(path, index, challenge)
{
	mTextures[0] = new Texture("Bosses.png", 0, 0, 64, 64);
	mTextures[1] = new Texture("Bosses.png", 64, 0, 64, 64);
	
	for (auto texture : mTextures) {
		texture->Parent(this);
		texture->Position(Vec2_Zero);
	}

	/*mTextures[1]->Parent(this);
	mTextures[1]->Position(Vec2_Zero);
	mTextures[0]->Parent(this);
	mTextures[0]->Position(Vec2_Zero);*/

	mType = Enemy::Boss;
}

Boss::~Boss()
{

}