#include "stdafx.h"
#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>


namespace SimE {
	TextureCache::TextureCache() {

	}


	TextureCache::~TextureCache() {

	}

	GLTexture TextureCache::getTexture(std::string filePath) {
		auto iter = _textureMap.find(filePath);

		// check if it is not in the map
		if (iter == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(filePath);

			_textureMap.insert(make_pair(filePath, newTexture));
			std::cout << "Loaded Cached Texture!\n";
			return newTexture;
		}
		//std::cout << "Used Cached Texture!\n";
		return iter->second; // first would be key, second is the GLTexture
	}
}