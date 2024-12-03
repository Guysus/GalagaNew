#include "Scoreboard.h"

void Scoreboard::ClearBoard() {
	for (auto texture : mScore) {
		delete texture;
	}

	mScore.clear();
}

Scoreboard::Scoreboard() : Scoreboard({230, 230, 230}) { }

Scoreboard::Scoreboard(SDL_Color color) {
	mColor = color;
	Score(0);
}

void Scoreboard::Score(int score) {
	ClearBoard();

	if (score == 0) {
		//The number 2 represents how many ZEROS we want to display
		for (int i = 0; i < 2; i++) {
			mScore.push_back(new Texture("0", "emulogic.ttf", 32, mColor));
			mScore[i]->Parent(this);
			mScore[i]->Position(Vector2(-32.0f * i, 0.0f));
		}
	}
	else {
		std::string str = std::to_string(score);
		unsigned lastIndex = (unsigned)str.length() - 1;
		
		for (int i = 0; i <= lastIndex; i++) {
			mScore.push_back(new Texture(str.substr(i, 1), "emulogic.ttf", 32, mColor));
			mScore[i]->Parent(this);
			mScore[i]->Position(Vector2(-32.0f * (lastIndex - i), 0.0f));
		}
	}
}

void Scoreboard::Render() {
	for (auto digit : mScore) {
		digit->Render();
	}
}

Scoreboard::~Scoreboard() {
	ClearBoard();
}