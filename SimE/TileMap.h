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

		std::vector<std::vector<int>*>* getLayers() { return &_layers; }

		void clearMap();
		void addLayer(std::string filepath);
	private:
		
		// an array that stores the CSV data
		std::string _filePathToLevel;

		std::vector<std::vector<int>*> _layers; // hash generated from level filepath, and CSV data
	};

}