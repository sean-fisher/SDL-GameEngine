#pragma once
#include <glm/glm.hpp>
#include "Directions.h"
#include <string>
#include "Sprite.h"
#include "Alive.h"
#include "Structs.h"
#include "Camera2D.h"

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
		void onCollide(Alive* other) override;

		// Getters
		float getWalkSpeed() { return m_walkSpeed; }
		float getRunSpeed() { return m_walkSpeed; }
		std::string getName() { return m_name; }

		glm::vec2 m_scale;

		Sprite* m_sprite;

		Collider* m_collider;

		bool m_canMove;
		bool m_canControl;

		Camera2D* cam;

	private:
		std::string m_name;
		float m_walkSpeed;
		float m_runSpeed;
		bool isRunning;
	};

}