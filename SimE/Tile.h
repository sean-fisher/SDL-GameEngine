#pragma once
#include <string>
#include "GLTexture.h"

namespace SimE {

	struct Tile {
		int id;
		int width;
		int height;
		std::string filepath;
		static GLTexture texture;
	};

}