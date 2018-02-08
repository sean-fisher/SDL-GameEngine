#include "stdafx.h"
#include "ResourceManager.h"

namespace SimE {
	TextureCache ResourceManager::_textureCache;

	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return _textureCache.getTexture(texturePath);
	}
}