#include "stdafx.h"
#include "Being.h"
#include "InputManager.h"
#include "SpriteLayers.h"
#include "Timer.h"

#include <SDL/SDL.h>

namespace SimE {
	Being::~Being() {
	}

	Being::Being(float walkSpeed, float runSpeed, int xPos, int yPos, GLTexture tex) : m_walkSpeed(walkSpeed),
			   m_runSpeed(runSpeed) {
		m_position = glm::vec2(xPos, yPos);

		m_sprite = new Sprite(&m_position, tex, 0/*depth*/, ColorRGBA8(255, 255, 255, 255));

		isRunning = false;

		m_canControl = true;
		m_canMove = true;
		m_collider = new SquareCollider(16, 32, &m_position, false);
		m_collider->setAlive(this);
		CollisionLayers::addCollider(m_collider);
		SpriteLayers::getInstance()->addToLayer(2, m_sprite);
	}
	
	bool Being::moveTo(glm::vec2 destination) {
		return false;
	}

	bool Being::faceDirection(glm::vec2 direction) {
		return false;
	}

	bool Being::faceDirection(SimE::Direction direction) {
		return false;
	}
	void Being::checkInput() {
		m_lastPosition = m_position;
		m_collider->setLastPos(m_position);
		if (m_canControl) {
			InputManager* im = InputManager::singleton();
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->up)) {
				//move up
				if (!isRunning) {
					m_position += glm::vec2(0, m_walkSpeed * Timer::delta());
				} else {
					m_position += glm::vec2(0, m_runSpeed * Timer::delta());
				}
			}
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->down)) {
				//move down
				if (!isRunning) {
					m_position += glm::vec2(0, -m_walkSpeed * Timer::delta());
				} else {
					m_position += glm::vec2(0, -m_runSpeed * Timer::delta());
				}
			}
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->left)) {
				//move left
				if (!isRunning) {
					m_position += glm::vec2(-m_walkSpeed * Timer::delta(), 0);
				} else {
					m_position += glm::vec2(-m_runSpeed * Timer::delta(), 0);
				}
			}
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->right)) {
				//move right
				if (!isRunning) {
					m_position += glm::vec2(m_walkSpeed * Timer::delta(), 0);
				} else {
					m_position += glm::vec2(m_runSpeed * Timer::delta(), 0);
				}
			}
			m_collider->setPos(m_position);
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->a)) {
				;
			}
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->b)) {
				;
			}
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->x)) {
				;
			}
			if (InputManager::singleton()->isKeyDown(
				InputManager::singleton()->y)) {
			}
			cam->setPosition(m_position);
		}
	}
	bool Being::beat() {
		checkInput();
		m_collider->setPos(m_position);
		m_collider->setLastPos(m_lastPosition);
		CollisionLayers::checkAllCollisionsFor(m_collider);
		return true;
	}

	void Being::onCollide(Alive * other) {
		//printf("Collision Override\n");
	}
}