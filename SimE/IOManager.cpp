#include "stdafx.h"
#include "IOManager.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "tinyxml2.h"
#include "ResourceManager.h"
#include <string.h>

namespace SimE {


	bool IOManager::readFileToBuffer(const std::string filePath,
		std::vector<unsigned char>& buffer) {
		std::ifstream file(filePath, std::ios::binary); // indicates we are reading a binary file
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}


		// how big is the file?
		// seek to the end
		file.seekg(0, std::ios::end);


		// get the file size
		int filesize = (int)file.tellg();
		file.seekg(0, std::ios::beg); // move back to beginning of file

		filesize -= (int)file.tellg(); // get rid of the file header

		buffer.resize(filesize);
		file.read((char*)&(buffer[0]), filesize);// convert vector to char*

		file.close();

		return true;
	}

	// the buffer contains the int values representing tiles
	bool IOManager::readTilemapLayer(const std::string filePath,
		std::vector<int>& buffer) {
		std::ifstream file(filePath, std::ifstream::in);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}
		buffer.clear();
		char character;
		std::string value;
		while (file >> std::noskipws >> character) {
			if (character != ',') {
				if (character == '\n') {
					buffer.push_back(std::stoi(value));
					buffer.push_back(-2); // EoL has been reached; -2 represents a newline
					value = "";
				} else {
					// form a string that represents the int value of a tile
					value += character;
				}
			} else {
				// the end of the value has been found; add the value to the buffer
				buffer.push_back(std::stoi(value));
				value = "";
			}
		}
		printf("Read tile map layer of size %d", buffer.size());
		file.close();
		
		return true;
	}

	bool IOManager::readTileset(const std::string filePath, std::unordered_map<int, GLTexture>& buffer) {

		tinyxml2::XMLDocument setInfo;
		setInfo.LoadFile(filePath.c_str());

		if (setInfo.Error() != tinyxml2::XML_SUCCESS) { 
			printf("Error: %i\n", setInfo.Error()); 
		}

		// get the root node and the name of the tileset
		const char* tilesetName;
		tinyxml2::XMLNode* rootNode = setInfo.FirstChild()->NextSibling();
		rootNode->ToElement()->QueryStringAttribute("name", &tilesetName);

		// get the number of tiles
		int tileCount;
		rootNode->ToElement()->QueryIntAttribute("tilecount", &tileCount);

		// load the first tile 
		auto tile = rootNode->FirstChildElement()->NextSibling();
		int id;
		int width;
		int height;
		const char* source;
		auto image = tile->FirstChild();
		tile->ToElement()->QueryIntAttribute("id", &id);
		image->ToElement()->QueryIntAttribute("width", &width);
		image->ToElement()->QueryIntAttribute("height", &height);
		image->ToElement()->QueryStringAttribute("source", &source);
		//printf("id: %d, width: %d, height: %d, source: %s\n", id, width, height, source);

		std::string realSource = "Assets/Sprites/" + (std::string) source;

		// when tiles are loaded, the id is used to find the GLTexture.
		GLTexture newTexture = ResourceManager::getTexture(realSource);
		newTexture.width = width;
		newTexture.height = height;
		buffer[id] = newTexture;
		for (int i = 1; i < tileCount; i++) {
			tile = tile->NextSibling();
			image = tile->FirstChild();
			tile->ToElement()->QueryIntAttribute("id", &id);
			image->ToElement()->QueryIntAttribute("width", &width);
			image->ToElement()->QueryIntAttribute("height", &height);
			image->ToElement()->QueryStringAttribute("source", &source);
			realSource = "Assets/Sprites/" + (std::string) source;
			newTexture = ResourceManager::getTexture(realSource);
			newTexture.width = width;
			newTexture.height = height;
			buffer[id] = newTexture;
		}
		printf("Loaded all %d sprites from %s!\n", tileCount, tilesetName);
		return false;
	}

	int IOManager::getHashFromFilepath(const std::string & filepath) {
		int hash = 0;
		for (size_t i = 0; i < filepath.length(); i++) {
			hash += filepath[i] * i;
		}
		return hash;
	}
}
