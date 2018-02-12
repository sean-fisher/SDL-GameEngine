#include "stdafx.h"
#include "SpriteLayers.h"
#include "Timer.h"
#include <iostream>
#include "GLTexture.h"
#include "SDLerrors.h"

namespace SimE {
	void SpriteLayers::addToLayer(int layer, Sprite* sprite) {
		(*_layers)[layer]->addSprite(sprite);
	}

	bool SpriteLayers::removeFromLayer(int layer, Sprite * sprite) {

		(*_layers)[layer]->removeSprite(sprite);

		return false;
	}

	void SpriteLayers::beginDrawing() {

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// enable the shader
		_colorProgram.use();
		glActiveTexture(GL_TEXTURE0);

		GLint textureLocation = _colorProgram.getUniformLocation("spriteSampler");
		glUniform1i(textureLocation, 0);

		GLuint timeLoc = _colorProgram.getUniformLocation("time");
		// send variable to shader
		glUniform1f(timeLoc, Timer::getTime());
	}

	void SpriteLayers::drawAll(Camera2D* cam, Camera2D* uiCam, Window* window) {
		//SDL_PollEvent(&_event);
		// Set the ortho camera matrix
		GLuint projLoc = _colorProgram.getUniformLocation("orthoProj");
		glm::mat4 camMatrix = cam->getCameraMatrix();
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camMatrix[0][0]);

		// draw every layer
		//_groundLayer.beginDrawing();
		SDL_RenderClear(window->getRenderer());


		for (size_t i = 0; i < _layers->size(); i++) {
			SpriteBatch* batch = (*_layers)[i];
			batch->beginDrawing();
			batch->drawAll(cam);
			batch->endDrawing();
		}

		drawUI(uiCam);
		/*
		SDL_SetRenderDrawColor(window->getRenderer(), 255, 255, 255, 0);
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				SDL_RenderDrawPoint(window->getRenderer(), 200+i, 200+j);
			}
		}
		SDL_SetRenderTarget(window->getRenderer(), NULL);*/
		//SDL_RenderPresent(window->getRenderer());
	}

	void SpriteLayers::endDrawing(Window* window) {
		// unbind the shader
		glBindTexture(GL_TEXTURE_2D, 0);

		// disable the shader
		_colorProgram.unuse();

	}
	void SpriteLayers::init(int numLayers) {
		createVertexArray();
		m_spriteFont = new SimE::SpriteFont("Assets/Fonts/ff6_font.ttf", 32);
		_colorProgram.initShaders();
		m_uiSpriteBatch = new SpriteBatch();
		m_uiSpriteBatch->init(_vao, _vbo, 10);

		_layers = new std::vector<SpriteBatch*>(numLayers);

		for (size_t i = 0; i < _layers->size(); i++) {
			(*_layers)[i] = new SpriteBatch();
			(*_layers)[i]->init(_vao, _vbo, i);
		}
	}

	void SpriteLayers::addMapToBatch(TileMap * map, Tileset* set) {

		// find sprites corresponding to int in map and add them to a sprite batch
		std::vector<std::vector<int>*>* layers = map->getLayers();
		int col = 0;
		int row = 0;
		int gridSquareSize = 8;
		// for each layer
		int numTilesAdded = 0;
		size_t layerNum = 0;
		for (; layerNum < layers->size(); layerNum++) {
			// the std::vector<int> in each layer is csv data (it->second is the CSV data)

			std::vector<int>* layer = (*layers)[layerNum]; // todo: the issue is that the layer, which should contain -1, 0, 0, etc. has the wrong info
			if (layer->size() > 8000) fatalError("Layer was not retrieved properly!");
			for (size_t i = 0; i < layer->size(); i++) {
				// for each id in the layer, add a sprite with texture being 
				int id = (*layer)[i];
				if (id != -1 && id != -2) {
					printf("Sprite: %d\n", id);
					GLTexture tex = set->getTextureFromID(id);
					printf("tex width: %d", tex.width);
					glm::vec2* tilePos = new glm::vec2(col * gridSquareSize, row * gridSquareSize);
					glm::vec2* uv = new glm::vec2(tex.width, tex.height);
					Sprite* tile = new Sprite(tilePos, tex, 0/*depth*/, ColorRGBA8(255, 255, 255, 255));
					addToLayer(layerNum, tile); /*TODO: MAKE work with all layers*/
				} else if (id == -2) {
					row--;
					col = 0;
				}
				col++;
				numTilesAdded++;
			}
			row = 0;
			col = 0;
		}
		printf("Added %d tiles to batch from %d layers!\n", numTilesAdded, layerNum);
	}

	bool SpriteLayers::hasLoadedTileset(int hash) {
		return _loadedTilesets.find(hash) != _loadedTilesets.end();
	}

	void SpriteLayers::addTileset(int hash, Tileset * tileset) {
		_loadedTilesets[hash] = tileset;
	}


	void SpriteLayers::createVertexArray() {
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// What kind of attributes?
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// let OpenGL know where vertex data is

		// this is the position attribute pointer
		glVertexAttribPointer(0,
			2, /*number of coordinates*/
			GL_FLOAT,
			GL_FALSE /* Does it use normalized coordinates?*/,
			sizeof(Vertex), /*stride: size of vertex struct*/
			(void*)offsetof(Vertex, position)/*offset from beginning of struct*/);

		// this is the color attribute pointer
		glVertexAttribPointer(1,
			4, /*number of coordinates*/
			GL_UNSIGNED_BYTE,
			GL_TRUE /*Converts from 0-255 to 0-1*/,
			sizeof(Vertex), /*stride: size of vertex struct*/
			(void*)offsetof(Vertex, color)/*offset from beginning of struct*/);

		// this is the UV attribute pointer
		glVertexAttribPointer(2,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteLayers::drawUI(Camera2D* uiCam) {

		GLuint projLoc = _colorProgram.getUniformLocation("orthoProj");
		glm::mat4 camMatrix = uiCam->getCameraMatrix();
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camMatrix[0][0]);
		m_uiSpriteBatch->begin();

		char message[256];
		sprintf_s(message, "FPS: %d", FPSLimiter::getFPS());

		m_spriteFont->draw(*m_uiSpriteBatch, message, glm::vec2(0, 0), glm::vec2(1), 0, ColorRGBA8(255, 255, 255, 255));

		m_uiSpriteBatch->end();
		m_uiSpriteBatch->renderBatch();
	}
}