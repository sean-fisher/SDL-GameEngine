#pragma once
#include <unordered_map>
#include "GLTexture.h"

namespace SimE {
	class Tileset {
	public:
		Tileset();
		~Tileset();
		// the filepath points to an xml file with a mapping of ints and .png files
		Tileset(const std::string filepath);

		GLTexture getTextureFromID(int hash);

	private:

		// GLTextures are small structs so it is fine to not use pointers here
		// since it is an unordered map, they are stored next to each other, so this is actually preferred
		static std::unordered_map<int, GLTexture> _idsAndTextures;
		
	};

}