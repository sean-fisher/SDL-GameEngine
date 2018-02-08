#pragma once

#include <GL/glew.h>
#include <string>

namespace SimE {
	class GLSLProgram {
		// this runs GLSL (OpenGL Shading Language) code (.vert and .frag files)
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void linkShaders();
		void addAttribute(const std::string& attributeName);
		void use();
		void unuse();
		void initShaders();

		GLuint getUniformLocation(const std::string& uniformName);

	private:
		int _numAttributes;
		GLuint _programID;
		GLuint _vertShaderID;
		GLuint _fragShaderID;
		void compileShader(const std::string& filePath, GLuint id);
	};
}