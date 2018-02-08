#include "stdafx.h"
#include "Tileset.h"
#include "IOManager.h"
#include "SpriteLayers.h"
#include <iostream>

namespace SimE {

	std::unordered_map<int, GLTexture> Tileset::_idsAndTextures;

	Tileset::Tileset() {
	}


	Tileset::~Tileset() {
	}

	Tileset::Tileset(const std::string filepath) {

		int hash = IOManager::getHashFromFilepath(filepath); // hash of the tileset

		if (!SpriteLayers::getInstance()->hasLoadedTileset(hash)) {

			// the set hasn't been loaded yet and must be loaded

			std::cout << "Loaded Tileset Data" << std::endl;

			// Reads values into this tileset's _idsAndTextures
			IOManager::readTileset(filepath, _idsAndTextures);

			// Sends this tileset to a collection of all loaded tilesets for future use
			SpriteLayers::getInstance()->addTileset(hash, this);
		} else {
			// TODO: return already loaded tileset address
		}

	}
	GLTexture Tileset::getTextureFromID(int hash) {
		return _idsAndTextures[hash];
	}
}