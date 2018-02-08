#pragma once

#include <GLM/glm.hpp>
#include <vector>

namespace SimE {


	class Collider {
	public:
		virtual bool testCollision(Collider* other) = 0;

		float getWidth() { return _width; }
		float getHeight() { return _height; }
		glm::vec2 getPos() { return _position; }
		glm::vec2 getLastPos() { return _lastPosition; }
		bool isStatic() { return _isStatic; }

		void setWidth(float width) { _width = width; }
		void setHeight(float height) { _height = height; }
		void setPos(glm::vec2 position) { _position = position; }
		void setLastPos(glm::vec2 position) { _lastPosition = position; }
		void setStatic(bool is_static) { _isStatic = is_static; }

	private:
		bool _isStatic;
		float _width;
		float _height;
		glm::vec2 _position;
		glm::vec2 _lastPosition;
	};

	class SquareCollider : public Collider {
	//	 _______
	//	|		|			x is the center
	//	|   x	|height
	//	|_______|
	//	  width
	public:

		bool testCollision(Collider* other) override;

		SquareCollider();
		SquareCollider(int width, int height, glm::vec2* position, bool isStatic);

	};

	class CollisionLayers {
	public:
		static void checkAllCollisionsFor(Collider* c);
		static void addCollider(Collider*);
		static CollisionLayers* singleton() {
			static CollisionLayers singleton;
			return &singleton;
		}
	private:


		std::vector<Collider*> colliders_nonstatic;
		std::vector<Collider*> colliders_static;

	};
}