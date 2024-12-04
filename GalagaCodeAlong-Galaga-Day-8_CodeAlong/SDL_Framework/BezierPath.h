#pragma once
#include "MathHelper.h"
#include <vector>

using namespace SDLFramework;

class BezierPath {
public:
	BezierPath();
	~BezierPath();

	void AddCurve(BezierCurve curve, int samples);
	void Sample(std::vector<Vector2>* samplePath);

private:
	std::vector<BezierCurve> mCurves;
	std::vector<int> mSamples;
};
