#pragma once
#include <string>
#include <unordered_map>
#include "Camera2D.h"

namespace SimE {
	class TileMap {
	public:
		TileMap();
		~TileMap();
		//creates a tilemap layer from a CSV exported from Tiled
		TileMap(std::string filepath);
		TileMap(std::string directory, std::string mapName);

		std::vector<std::vector<int>*>* getLayers() { return &m_layers; }

		void clearMap();
		void addLayer(std::string filepath);
	private:
		

		std::vector<std::vector<int>*> m_layers; // hash generated from level filepath, and CSV data
	};

}