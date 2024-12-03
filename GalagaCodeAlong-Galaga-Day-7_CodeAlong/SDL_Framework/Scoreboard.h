#pragma once
#include "Texture.h"
#include <vector>

using namespace SDLFramework;

class Scoreboard : public GameEntity {
public:
	Scoreboard();
	Scoreboard(SDL_Color color);
	~Scoreboard();

	void Score(int score);

	void Render() override;

private:
	void ClearBoard();

	std::vector<Texture*> mScore;
	SDL_Color mColor;
};