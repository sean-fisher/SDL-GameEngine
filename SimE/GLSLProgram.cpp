#include "stdafx.h"
#include "GLSLProgram.h"
#include "SDLerrors.h"
#include <fstream>
#include <vector>
#include <iostream>


namespace SimE {
	// Uses initialization list
	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0),
		_vertShaderID(0), _fragShaderID(0) {
	}



	GLSLProgram::~GLSLProgram() {

	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath,
		const std::string& fragmentShaderFilePath) {

		_programID = glCreateProgram(); // must be called before adding attributes


		_vertShaderID = glCreateShader(GL_VERTEX_SHADER);

		if (_vertShaderID == 0) {
			fatalError("Vertex shader creation failed");
		}
		_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragShaderID == 0) {
			fatalError("Fragment shader creation failed");
		}

		compileShader(vertexShaderFilePath, _vertShaderID);
		compileShader(fragmentShaderFilePath, _fragShaderID);
	}
	void GLSLProgram::linkShaders() {//Vertex and fragment shaders are successfully compiled.

									 //Attach our shaders to our program
		glAttachShader(_programID, _vertShaderID);
		glAttachShader(_programID, _fragShaderID);

		//Link our program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders either.
			glDeleteShader(_vertShaderID);
			glDeleteShader(_fragShaderID);

			//Use the infoLog as you see fit.
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link!");

			//In this simple program, we'll just leave
			return;
		}

		//Always detach shaders after a successful link.
		glDetachShader(_programID, _vertShaderID);
		glDetachShader(_programID, _fragShaderID);
		glDeleteShader(_vertShaderID);
		glDeleteShader(_fragShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName) {

		glBindAttribLocation(_programID,
			_numAttributes++,
			attributeName.c_str());
	}


	void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {

		// load code from .frag and .vert
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open " + filePath);
		}
		// load into a single string
		std::string fileContents = "";
		std::string line;
		while (std::getline(vertexFile, line)) {
			fileContents += line + "\n";
		}

		vertexFile.close();

		const char* contentsPointer = fileContents.c_str();
		glShaderSource(id, 1, &contentsPointer, nullptr);

		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");
			return;
		}
	}

	void GLSLProgram::use() {
		glUseProgram(_programID);

		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}
	void GLSLProgram::unuse() {

		glUseProgram(0);

		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	GLuint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint loc = glGetUniformLocation(_programID, uniformName.c_str());
		if (loc == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return loc;
	}
	void GLSLProgram::initShaders() {
		compileShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
		addAttribute("vertexPosition");
		addAttribute("vertexColor");
		addAttribute("vertexUV");
		linkShaders();
	}
}
