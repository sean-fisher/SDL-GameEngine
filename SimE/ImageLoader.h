#pragma once
#include "GLTexture.h"
#include <string>

namespace SimE {
	class ImageLoader {
	public:
		static GLTexture loadPNG(std::string filepath);
	};

}