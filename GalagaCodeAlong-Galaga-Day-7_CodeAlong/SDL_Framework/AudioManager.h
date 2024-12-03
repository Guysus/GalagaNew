#pragma once
#include "AssetManager.h"

namespace SDLFramework {

	class AudioManager {
	public:
		static AudioManager* Instance();
		static void Release();

		void PlayMusic(std::string filename, int loops = -1);
		void PlayMusic(Mix_Music* music, int loops = -1);
		void PauseMusic();
		void ResumeMusic();

		void PlaySFX(std::string filename, int loops = 0, int channel = -1);
		void PlaySFX(Mix_Chunk* sfx, int loops = 0, int channel = -1);

	private:
		AudioManager();
		~AudioManager();

		static AudioManager* sInstance;
		AssetManager* mAssetManager;
	};
}