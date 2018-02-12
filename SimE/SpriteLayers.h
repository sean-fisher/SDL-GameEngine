#pragma once
#include <unordered_map>
#include "SpriteBatch.h"
#include "Sprite.h"
#include "Camera2D.h"
#include "TileMap.h"
#include "Tileset.h"
#include "Window.h"
#include "SpriteFont.h"

namespace SimE {
	enum SpriteLayer {
		GROUND = 0,
		CHARACTER1 = 1,
		CHARACTER2 = 2,
		EFFECT = 3,
		AIR1 = 4,
		AIR2 = 5,
		EXTRA1 = 6,
		EXTRA2 = 7,
	};

	class SpriteLayers {
	public:
		static SpriteLayers* getInstance() {
			static SpriteLayers singleton;
			return &singleton;
		}
		void addToLayer(int layer, Sprite* sprite);
		bool removeFromLayer(int layer, Sprite* sprite);
		void beginDrawing();
		void drawAll(Camera2D* cam, Window* window);
		void endDrawing(Window* window);
		void init(int numLayers);
		void addMapToBatch(TileMap* map, Tileset* set);
		bool hasLoadedTileset(int hash);
		void addTileset(int hash, Tileset* tileset);
	private:
		void createVertexArray();
		void drawUI();

		std::vector<SpriteBatch*>* _layers;

		SpriteBatch _currentMap;
		std::unordered_map<int, Tileset*> _loadedTilesets;

		GLSLProgram _colorProgram;

		SDL_Event _event;

		// Vertex Buffer Object
		GLuint _vbo;

		// Vertex Array Object
		GLuint _vao;

		SpriteBatch m_uiSpriteBatch;
		SpriteFont* m_spriteFont;
	};
}