#include "stdafx.h"
#include "ImageLoader.h"
#include "PicoPNG.h"
#include "IOManager.h"
#include "SDLerrors.h"

namespace SimE {
	GLTexture ImageLoader::loadPNG(std::string filepath) {
		GLTexture texture = {};//initializes everything to 0

		std::vector<unsigned char> in;
		std::vector<unsigned char> out;

		unsigned long width, height;

		if (!IOManager::readFileToBuffer(filepath, in)) {
			fatalError("Failed to load PNG file to buffer");
		}

		int errorCode = decodePNG(out, width, height, &in[0], in.size());

		if (errorCode != 0) {
			fatalError("Decode PNG failed with error code " + errorCode);
		}

		glGenTextures(1, &(texture.id));

		// upload the data
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &out[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // set to GL_LINEAR for blur
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		return texture;
	}
}