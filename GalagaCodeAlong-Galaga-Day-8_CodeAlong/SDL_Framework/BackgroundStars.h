#pragma once
#include "StarLayer.h"

class BackgroundStars {
public:
	static BackgroundStars* Instance();
	static void Release();

	void Scroll(bool scroll);

	void Update();
	void Render();

private:
	BackgroundStars();
	~BackgroundStars();

	static BackgroundStars* sInstance;
	static const int LAYER_COUNT = 3;
	StarLayer* mLayers[LAYER_COUNT];
};