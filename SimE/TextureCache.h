#pragma once
#include <map>
#include "GLTexture.h"

namespace SimE {
	class TextureCache {
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string filePath);
	private:
		// filepaths and textures (i think)
		std::map<std::string, GLTexture> _textureMap;
	};

}