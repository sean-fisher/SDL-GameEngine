#pragma once
#include <GL/glew.h>
#include "GLTexture.h"
#include <string>
#include <glm/glm.hpp>
#include "Vertex.h"

namespace SimE {
	class Sprite {
	public:
		Sprite();
		Sprite(glm::vec2* pos, glm::vec2 uv, std::string filepath, float depth, ColorRGBA8 color);
		Sprite(glm::vec2* pos, 
			GLTexture texture,
			float depth,
			ColorRGBA8 color);
		Sprite(glm::vec2 pos,
			glm::vec2 uv,
			GLTexture texture,
			float depth,
			ColorRGBA8 color);
		//Sprite(glm::vec2* pos, glm::vec2 uv, std::string filepath, float depth, Color color, glm::vec2 scale);
		~Sprite();

		/*bool Sprite::operator==(const Sprite &other) const {
			return _texture.id == other._texture.id && _pos == other._pos;
		}*/

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw(glm::vec2* pos);
		void draw();
		GLuint _shaderID;
		glm::vec2* _pos; // This is a pointer so it can keep track of its position even if the object associated is moving
		glm::vec2 _uv;
		GLTexture _texture;
		float _depth;
		ColorRGBA8 _color;
	private:
		float _x;
		float _y;
		float _width;
		float _height;
		GLuint _vboID;
	};
}