#pragma once
#include <vector>
#include "GLTexture.h"
#include "Sprite.h"
#include <unordered_map>

namespace SimE {
	class IOManager // a static class
	{
	public:
		static bool readFileToBuffer(const std::string filePath,
			std::vector<unsigned char>& buffer);

		static bool readTilemapLayer(const std::string filePath,
			std::vector<int>& buffer);
		static bool readTileset(const std::string filePath,
			std::unordered_map<int, GLTexture>& buffer);
		static int getHashFromFilepath(const std::string & filepath);

	};

}