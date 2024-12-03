#pragma once
#include <SDL.h>
#include "MathHelper.h"
#include <string>

namespace SDLFramework {

	class InputManager {
	public:
		enum MouseButton { Left = 0, Right, Middle, Back, Forward };

		static InputManager* Instance();
		static void Release();

		//Mouse Functionality
		bool MouseButtonDown(MouseButton button);
		bool MouseButtonPressed(MouseButton button);
		bool MouseButtonReleased(MouseButton button);

		Vector2 MousePosition();

		//Keyboard functionality
		bool KeyDown(SDL_Scancode scancode);
		bool KeyPressed(SDL_Scancode scancode);
		bool KeyReleased(SDL_Scancode scancode);

		void Update();
		void UpdatePrevInput();

	private:
		static InputManager* sInstance;

		InputManager();
		~InputManager();

		//Mouse members
		Uint32 mPrevMouseState;
		Uint32 mMouseState;

		int mMouseXPos;
		int mMouseYPos;

		//Keyboard members
		const Uint8* mKeyboardState;
		Uint8* mPrevKeyboardState;
		int mKeyLength;
	};
}