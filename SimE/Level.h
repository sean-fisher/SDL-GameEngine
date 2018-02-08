#pragma once

#include <vector>
#include <string>

#include "TileMap.h"
#include "Tileset.h"


namespace SimE {
	class Level {
	public:
		Level();
		~Level();

		std::string _levelName;
		TileMap _tm;
		std::vector<Tileset> _tileSetsNeeded;
	};

}