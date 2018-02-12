#include "stdafx.h"
#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>
#include "Time.h"
#include "ResourceManager.h"
#include "Camera2D.h"

#define _CRTDBG_MAP_ALLOC  

namespace SimE {


	SpriteBatch::SpriteBatch() {
	}


	SpriteBatch::~SpriteBatch() {
	}


	void SpriteBatch::init(GLuint vao, GLuint vbo, int depth) {
		m_vao = vao;
		m_vbo = vbo;
		m_layerNum = depth;
	}

	void SpriteBatch::begin() {
		m_sortType = GlyphSortType::TEXTURE;
		m_renderBatches.clear();

		m_glyphs.clear();
	}

	void SpriteBatch::end() {
		m_glyphPointers.resize(m_glyphs.size());
		for (size_t i = 0; i < m_glyphs.size(); i++) {
			m_glyphPointers[i] = &m_glyphs[i];
		}

		// make sure the glyphs are sorted for efficiency
		sortGlyphs();

		createRenderBatches();
	}
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {

		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, glm::vec2& scale) {
/*		Glyph* newGlyph = new Glyph;

		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x * scale.x, (destRect.y + destRect.w) * scale.y);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x * scale.x, destRect.y * scale.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);


		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition((destRect.x + destRect.z) * scale.x, destRect.y * scale.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition((destRect.x + destRect.z) * scale.x, (destRect.y + destRect.w) * scale.y);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);*/
	}

	void SpriteBatch::drawAll(Camera2D* cam) {



		begin(); // 3 is GlyphSortType::TEXTURE

		// pos is xcoor, ycoor, width, height
		glm::vec4 pos(0, 0, 100, 100);
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		
		SimE::ColorRGBA8 color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 255;
		if (m_sprites.size() > 0) {

			_currentShaderID = m_sprites[0]->_shaderID;
			pos.x = m_sprites[0]->_pos->x - m_sprites[0]->_texture.width / 2;
			pos.y = m_sprites[0]->_pos->y - m_sprites[0]->_texture.height / 2;
			pos.z = m_sprites[0]->_uv.x;
			pos.w = m_sprites[0]->_uv.y;
			uv.x = m_sprites[0]->_uv.x;
			uv.y = m_sprites[0]->_uv.y;

			glm::vec2 aliveDims(/*radius*/ 4);

			if (cam->canBeSeen(*m_sprites[0]->_pos, m_sprites[0]->_texture.width, m_sprites[0]->_texture.height)) {
				draw(pos, uv, (m_sprites[0])->_texture.id, 0, color);
			}
			for (size_t i = 1; i < m_sprites.size(); i++) {

				// used for occlusion
				if (cam->canBeSeen(*m_sprites[i]->_pos, m_sprites[i]->_texture.width, m_sprites[i]->_texture.height)) {
					if (m_sprites[i]->_shaderID != _currentShaderID) {
						_currentShaderID = m_sprites[i]->_shaderID;
					}

					pos.x = m_sprites[i]->_pos->x - 
						m_sprites[i]->_texture.width / 2;									   
					pos.y = m_sprites[i]->_pos->y -
							m_sprites[i]->_texture.height / 2;
					pos.z = m_sprites[i]->_uv.x;
					pos.w = m_sprites[i]->_uv.y;
					uv.x = m_sprites[i]->_uv.x;
					uv.y = m_sprites[i]->_uv.y;
					draw(pos, uv, m_sprites[i]->_texture.id, (float) m_layerNum, color);
				}
			}

		}

	}

	void SpriteBatch::renderBatch() {

		glBindVertexArray(m_vao);

		for (size_t i = 0; i < m_renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}

		glBindVertexArray(0);

	}


	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(m_glyphPointers.size() * 6); // each rectangular glyph has 6 vertices

		if (m_glyphPointers.empty()) {
			return;
		}

		int offset = 0;
		int currentVertex = 0;

		m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture);
		vertices[currentVertex++] = m_glyphPointers[0]->topLeft;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomLeft;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[0]->topRight;
		vertices[currentVertex++] = m_glyphPointers[0]->topLeft;
		offset += 6;


		for (size_t cg = 1; cg < m_glyphPointers.size(); cg++) {

			if (m_glyphPointers[cg]->texture != m_glyphPointers[cg - 1]->texture) {

				m_renderBatches.emplace_back(offset, 6, m_glyphPointers[cg]->texture);
			} else {

				m_renderBatches.back().numVertices += 6;
			}
			vertices[currentVertex++] = m_glyphPointers[cg]->topLeft;
			vertices[currentVertex++] = m_glyphPointers[cg]->bottomLeft;
			vertices[currentVertex++] = m_glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = m_glyphPointers[cg]->bottomRight;
			vertices[currentVertex++] = m_glyphPointers[cg]->topRight;
			vertices[currentVertex++] = m_glyphPointers[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// orphan the buffer
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(Vertex),
			nullptr,
			GL_DYNAMIC_DRAW);

		// upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::sortGlyphs() {

		switch (m_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);

	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {

		return (a->texture < b->texture);
	}

	void SpriteBatch::addSprite(Sprite * sprite) {
		m_sprites.emplace_back(sprite);
	}

	void SpriteBatch::removeSprite(Sprite * sprite) {

		size_t i = 0;
		while (i < m_sprites.size() && m_sprites[i++] != sprite) {}
		if (i <= m_sprites.size()) {
			m_sprites.erase(m_sprites.begin() + i - 1);
		}
	}

	void SpriteBatch::clearSprites() {
	}


	void SpriteBatch::initShaders() {
		m_colorProgram.compileShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
		m_colorProgram.addAttribute("vertexPosition");
		m_colorProgram.addAttribute("vertexColor");
		m_colorProgram.addAttribute("vertexUV");
		m_colorProgram.linkShaders();
	}


	void SpriteBatch::beginDrawing() {

		// enable the shader
		/*_colorProgram.use();
		glActiveTexture(GL_TEXTURE0);

		GLint textureLocation = _colorProgram.getUniformLocation("spriteSampler");
		glUniform1i(textureLocation, 0);

		GLuint timeLoc = _colorProgram.getUniformLocation("time");
		// send variable to shader
		glUniform1f(timeLoc, Time::getTime());*/
	}

	void SpriteBatch::endDrawing() {

		end();
		renderBatch();

	}
	Glyph::Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint Texture, float Depth, const ColorRGBA8 & color) : depth(Depth), texture(Texture)
	{
		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}
}