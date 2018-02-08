// SimE.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#include "SimE_class.h"

namespace SimE {
	int init() {

		// Initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}
