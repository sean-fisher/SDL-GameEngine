#include "stdafx.h"
#include "sprite.h"
#include "Vertex.h"
#include <cstddef>
#include "ResourceManager.h"

namespace SimE {
	Sprite::Sprite() {
		_vboID = 0;
	}

	Sprite::Sprite(glm::vec2* pos, glm::vec2 uv, std::string filepath , float depth, Color color) {

		_pos = pos;

		_depth = depth;
		_color = color;
		if (filepath != "") {
			_texture = SimE::ResourceManager::getTexture(filepath);
		} else {
			_texture = SimE::ResourceManager::getTexture("Assets/Test/fun.png");

		}
		_uv.x = (float)_texture.width;
		_uv.y = (float)_texture.height;
	}
	/*Sprite::Sprite(glm::vec2* pos, glm::vec2 uv, std::string filepath, float depth, Color color, glm::vec2 scale) {

		_pos = pos;

		_depth = depth;
		_color = color;
		if (filepath != "") {
			_texture = SimE::ResourceManager::getTexture(filepath);
		} else {
			_texture = SimE::ResourceManager::getTexture("Assets/Test/fun.png");

		}
		_uv.x = (float)_texture.width * scale.x;
		_uv.y = (float)_texture.height * scale.y;
	}*/

	Sprite::Sprite(glm::vec2* pos, GLTexture texture, float depth, Color color) {
		_pos = pos;
		_uv.x = (float) texture.width;
		_uv.y = (float) texture.height;
		_texture = texture;
		_depth = depth;
		_color = color;
	}

	Sprite::Sprite(glm::vec2 pos, glm::vec2 uv, GLTexture texture, float depth, Color color) {
		_pos = &pos;
		_uv = uv;
		_texture = texture;
		_depth = depth;
		_color = color;
	}

	Sprite::~Sprite() {
		// free the vertex buffer
		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Sprite::init(float x, float y,
		float width, float height, std::string texturePath) {
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		// generate the vertex buffer
		if (_vboID == 0) {
			// sets to unique buffer id
			glGenBuffers(1, &_vboID);
		}

		// create space for vertices
		Vertex vertexData[6]; // space for 6 vertices for a quad

							  // first triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1, 1);
		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0, 1);
		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0, 0);

		// second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0, 0);
		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1, 0);
		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1, 1);

		for (int i = 0; i < 6; i++) {
			// tint the sprite
			vertexData[i].setColor(255, 255, 255, 255);
		}

		// bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER,
			/*amount of data*/ sizeof(vertexData),
			vertexData,
			/*usage*/ GL_STATIC_DRAW);

		// unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw(glm::vec2* pos) {

		glBindTexture(GL_TEXTURE_2D, (_texture).id);

		// we can only have one buffer open at a time
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);



		// Draw the data
		glDrawArrays(GL_TRIANGLES/*mode*/,
			0 /*What's the first element?*/,
			6 /*how many elements are we drawing?*/);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Sprite::draw() {

	}
}