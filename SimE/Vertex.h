#pragma once

#include <GL/glew.h>

namespace SimE {
	struct Position {
		float x;
		float y;
	};

	struct Color {
		Color::Color() {};
		Color::Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a) 
			: r(r), g(g), b(b), a(a) {};
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			r = r;
			g = g;
			b = b;
			a = a;
		}
		void setColor(int r, int g, int b, int a) {
			r = r;
			g = g;
			b = b;
			a = a;
		}
	};

	struct UV {

		float u;
		float v;
	};

	struct Vertex {
		Position position;
		Color color;
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
		void setColor(int r, int g, int b, int a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}
	};
}