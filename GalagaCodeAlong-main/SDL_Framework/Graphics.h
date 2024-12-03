#pragma once
#include <SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include <iostream>
#include <string>

namespace SDLFramework {
	class Graphics {
	public:
		static const int SCREEN_WIDTH = 600;
		static const int SCREEN_HEIGHT = 800;

		static Graphics* Instance();
		//Handle releasing (uninitializing) memory. ONLY HANDLES OUR WINDOW AND RENDERER
		static void Release();
		static bool Initialized();

		SDL_Texture* LoadTexture(std::string path);

		void DrawTexture(SDL_Texture* texture, SDL_Rect* srcRect = nullptr, SDL_Rect* dst_Rect = nullptr,
			float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);

		SDL_Texture* CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color);

		//Cleanup that happens to the area in memory that draws the next frame
		void ClearBackBuffer();
		void Render();

		Graphics();
		~Graphics();

		bool Init();

	private:
		//This is going to hold the ONE instance of our Graphics class
		static Graphics* sInstance;
		static bool sInitialized;
		SDL_Window* mWindow = nullptr;
		SDL_Renderer* mRenderer = nullptr;
	};
}