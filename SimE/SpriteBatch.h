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

	class Glyph {
	public:
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		
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
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, glm::vec2& scale);
		void drawAll(Camera2D* cam);
		void addSprite(Sprite* sprite);
		void removeSprite(Sprite* sprite);
		void clearSprites();
		void renderBatch();
		void beginDrawing();
		void endDrawing();

		// Getters
		GLSLProgram* getGLSLProgram() { return &m_colorProgram; }

		GLuint _currentShaderID;

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();
		void initShaders();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GlyphSortType m_sortType;


		std::vector<Glyph> m_glyphs;
		std::vector<Glyph*> m_glyphPointers;
		std::vector<Sprite*> m_sprites;
		std::vector<RenderBatch> m_renderBatches;

		GLSLProgram m_colorProgram;

		GLuint m_vao;
		GLuint m_vbo;

		int m_layerNum; // low numbers are drawn first
	};

}