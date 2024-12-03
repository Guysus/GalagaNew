#pragma once
#include "Star.h"

class StarLayer {
public:
	StarLayer(int layer);
	~StarLayer();

	void Update();
	void Render();

private:
	static const int STAR_COUNT = 30;
	Star* mStars[STAR_COUNT];
};