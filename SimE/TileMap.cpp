#include "TileMap.h"
#include "IOManager.h"
#include "stdafx.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace SimE {
	
	// from http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
	struct path_leaf_string {
		std::string operator()(const boost::filesystem::directory_entry& entry) const {
			return entry.path().leaf().string();
		}
	};

	void read_directory(const std::string& name, std::vector<std::string> &v) {
		boost::filesystem::path p(name);
		boost::filesystem::directory_iterator start(p);
		boost::filesystem::directory_iterator end;
		std::transform(start, end, std::back_inserter(v), path_leaf_string());
	}// end of cited section

	TileMap::TileMap() {
	}


	TileMap::~TileMap() {
		clearMap();
		//delete &_layers;
	}

	TileMap::TileMap(std::string filepath) {

		// the hash of the layer should be unnecessary since only one map is loaded at a time
		//int hash = IOManager::getHashFromFilepath(filepath);
		/*		if (_layers.find(hash) == _layers.end()) {
		// the map hasn't been loaded yet and must be loaded*/


		std::cout << "Loaded Level Data" << std::endl;
		std::vector<int>* in = new std::vector<int>();



		IOManager::readTilemapLayer(filepath, *in);
		m_layers.push_back(in);
		/*} else {
		// TODO
		}*/
	}

	TileMap::TileMap(std::string directory, std::string mapName) {
		std::vector<std::string> filenames;
		
		// find all the files in the target directory
		read_directory(directory, filenames);


		std::vector<std::string> relevantFilenames;
		// find the files with given map name.
		// this way we can get all the separate layers.
		for (size_t i = 0; i < filenames.size(); i++) {
			bool one = filenames[i].find(mapName) != std::string::npos;
			bool two = filenames[i].find(".csv") != std::string::npos;
			if (one && two) {
				relevantFilenames.push_back(filenames[i]);
			}
		}

		// read each layer into a sprite batch
		for (size_t i = 0; i < relevantFilenames.size(); i++) {

			std::cout << "Loaded Level Data: " 
				<< relevantFilenames[i] << std::endl;
			std::vector<int>* in = new std::vector<int>();

			IOManager::readTilemapLayer(directory + relevantFilenames[i], *in);
			m_layers.push_back(in);
		}
	}
	void TileMap::clearMap() {
		for (size_t i = 0; i < m_layers.size(); i++) {
			delete m_layers[i];
		}
	}
	void TileMap::addLayer(std::string filepath) {

		std::cout << "Loaded Level Data" << std::endl;
		std::vector<int>* in = new std::vector<int>();



		IOManager::readTilemapLayer(filepath, *in);
		m_layers.push_back(in);
	}
}