#pragma once
#include <GL/glew.h>
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>
#include "Sprite.h"
#include "GLSLProgram.h"
#include "Camera2D.h"
#include "GLTexture.h"
#include "TileMap.h"

namespace SimE {

	enum class GlyphSortType {
		TEXTURE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,

	};

	struct Glyph {
	public:
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : offset(offset), numVertices(numVertices), texture(texture) {

		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	private:

	};

	class SpriteBatch {
	public:
		SpriteBatch();
		~SpriteBatch();

		void init(GLuint vao, GLuint vbo, int numLayers);
		void begin();//GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
		void drawAll(Camera2D* cam);
		void addSprite(Sprite* sprite);
		void removeSprite(Sprite* sprite);
		void clearSprites();
		void renderBatch();
		void beginDrawing();
		void endDrawing();

		// Getters
		GLSLProgram* getGLSLProgram() { return &_colorProgram; }

		GLuint _currentShaderID;

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();
		void initShaders();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GlyphSortType _sortType;


		std::vector<Glyph*> _glyphs;
		std::vector<Sprite*> _sprites;
		std::vector<RenderBatch> _renderBatches;

		GLSLProgram _colorProgram;

		GLuint _vao;
		GLuint _vbo;

		int _layerNum; // low numbers are drawn first
	};

}