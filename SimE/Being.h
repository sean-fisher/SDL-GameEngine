#pragma once
#include <glm/glm.hpp>
#include "Directions.h"
#include <string>
#include "Sprite.h"
#include "Alive.h"
#include "Structs.h"

namespace SimE {
	class Being : public Alive {
		// This a character

	public:
		Being(float walkSpeed, float runSpeed, int xPos, int yPos, GLTexture tex);
		~Being();
		virtual bool moveTo(glm::vec2 destination);
		virtual bool faceDirection(glm::vec2 direction);
		virtual bool faceDirection(SimE::Direction direction);
		void checkInput();

		bool beat() override;

		// Getters
		float getWalkSpeed() { return _walkSpeed; }
		float getRunSpeed() { return _walkSpeed; }
		std::string getName() { return _name; }

		glm::vec2 _position;
		glm::vec2 _lastPosition;
		glm::vec2 _scale;

		Sprite* _sprite;

		Collider* _collider;

		bool _canMove;
		bool _canControl;

	private:
		std::string _name;
		float _walkSpeed;
		float _runSpeed;
		bool isRunning;
	};

}