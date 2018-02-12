#include "stdafx.h"
#include "Window.h"
#include "SDLerrors.h"

namespace SimE {
	Window::Window() {
	}


	Window::~Window() {
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE)
			flags |= SDL_WINDOW_HIDDEN;

		if (currentFlags & FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		if (currentFlags & BORDERLESS)
			flags |= SDL_WINDOW_BORDERLESS;

		// create, name, and position an SDL window
		_sdlWindow =
			SDL_CreateWindow(windowName.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				screenWidth,
				screenHeight,
				flags);
		if (_sdlWindow == nullptr) {
			fatalError("SDL Window couldn't be created");
		}

		// create an openGL context
		SDL_GLContext context = SDL_GL_CreateContext(_sdlWindow);
		if (context == nullptr) {
			fatalError("SDL_GL context could not be created");
		}

		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize GLEW");
		}

		// create a renderer for drawing without sprites
		_renderer = SDL_CreateRenderer(_sdlWindow, -1, 0);

		printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

		// RGBA, 0 to 1. Set background color
		glClearColor(0.0f, 0.0f, 1.0f, 1.0);

		SDL_GL_SetSwapInterval(0); //enables VSync; will limit FPS to 60 on 60Hz monitors

		// enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}
	void Window::swapBuffer() {

		SDL_GL_SwapWindow(_sdlWindow);

	}
}