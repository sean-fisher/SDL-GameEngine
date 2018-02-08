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
		_vao = vao;
		_vbo = vbo;
		_layerNum = depth;
	}

	void SpriteBatch::begin() {
		_sortType = GlyphSortType::TEXTURE;
		_renderBatches.clear();
		for (size_t i = 0; i < _glyphs.size(); i++) {
			delete _glyphs[i];
		}
		_glyphs.clear();
	}

	void SpriteBatch::end() {

		// make sure the glyphs are sorted for efficiency
		sortGlyphs();

		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) {
		Glyph* newGlyph = new Glyph;

		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);


		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::drawAll(Camera2D* cam) {



		begin(); // 3 is GlyphSortType::TEXTURE

		// pos is xcoor, ycoor, width, height
		glm::vec4 pos(0, 0, 100, 100);
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		
		SimE::Color color;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 255;
		if (_sprites.size() > 0) {

			_currentShaderID = _sprites[0]->_shaderID;
			pos.x = _sprites[0]->_pos->x;
			pos.y = _sprites[0]->_pos->y;
			pos.z = _sprites[0]->_uv.x;
			pos.w = _sprites[0]->_uv.y;
			uv.x = _sprites[0]->_uv.x;
			uv.y = _sprites[0]->_uv.y;

			glm::vec2 aliveDims(/*radius*/ 4);

			if (cam->canBeSeen(*_sprites[0]->_pos, aliveDims)) {
				draw(pos, uv, (_sprites[0])->_texture.id, 0, color);
			}
			for (size_t i = 1; i < _sprites.size(); i++) {

				if (cam->canBeSeen(*_sprites[i]->_pos, aliveDims)) {
					if (_sprites[i]->_shaderID != _currentShaderID) {
						_currentShaderID = _sprites[i]->_shaderID;
					}

					pos.x = _sprites[i]->_pos->x;
					pos.y = _sprites[i]->_pos->y;
					pos.z = _sprites[i]->_uv.x;
					pos.w = _sprites[i]->_uv.y;
					uv.x = _sprites[i]->_uv.x;
					uv.y = _sprites[i]->_uv.y;
					draw(pos, uv, _sprites[i]->_texture.id, _layerNum, color);
				}
			}

		}

	}

	void SpriteBatch::renderBatch() {

		glBindVertexArray(_vao);

		for (size_t i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}

		glBindVertexArray(0);

	}


	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(_glyphs.size() * 6); // each rectangular glyph has 6 vertices

		if (_glyphs.empty()) {
			return;
		}

		int offset = 0;
		int currentVertex = 0;

		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
		vertices[currentVertex++] = _glyphs[0]->topLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomLeft;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->bottomRight;
		vertices[currentVertex++] = _glyphs[0]->topRight;
		vertices[currentVertex++] = _glyphs[0]->topLeft;
		offset += 6;


		for (size_t cg = 1; cg < _glyphs.size(); cg++) {

			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {

				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			} else {

				_renderBatches.back().numVertices += 6;
			}
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomLeft;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->bottomRight;
			vertices[currentVertex++] = _glyphs[cg]->topRight;
			vertices[currentVertex++] = _glyphs[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

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

		switch (_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
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
		_sprites.emplace_back(sprite);
	}

	void SpriteBatch::removeSprite(Sprite * sprite) {

		size_t i = 0;
		while (i < _sprites.size() && _sprites[i++] != sprite) {}
		if (i <= _sprites.size()) {
			_sprites.erase(_sprites.begin() + i - 1);
		}
	}

	void SpriteBatch::clearSprites() {
	}


	void SpriteBatch::initShaders() {
		_colorProgram.compileShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
		_colorProgram.addAttribute("vertexPosition");
		_colorProgram.addAttribute("vertexColor");
		_colorProgram.addAttribute("vertexUV");
		_colorProgram.linkShaders();
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
}