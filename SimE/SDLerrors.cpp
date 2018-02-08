#include "stdafx.h"
#include "SDLerrors.h"
#include <string>
#include <iostream>
#include "SDL/SDL.h"

namespace SimE {
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "enter any key to quit";

		int temp;
		std::cin >> temp;
		SDL_Quit();
		exit(1);
	}
}