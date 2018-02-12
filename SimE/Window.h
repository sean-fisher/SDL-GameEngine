#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace SimE {
	enum WindowFlags {
		INVISIBLE = 0x1,
		FULLSCREEN = 0x2, BORDERLESS = 0x4
	};

	class Window {
	public:
		Window();
		~Window();

		int create(std::string, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth() { return _screenWidth; }
		int getScreenHeight() { return _screenHeight; }
		SDL_Renderer* getRenderer() { return _renderer; }
	private:
		SDL_Window* _sdlWindow;
		SDL_Renderer* _renderer;
		int _screenWidth;
		int _screenHeight;
	};

}