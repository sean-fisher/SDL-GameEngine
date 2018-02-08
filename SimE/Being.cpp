#include "stdafx.h"
#include "Being.h"
#include "InputManager.h"
#include "SpriteLayers.h"

#include <SDL/SDL.h>

namespace SimE {
	Being::~Being() {
	}

	Being::Being(float walkSpeed, float runSpeed, int xPos, int yPos, GLTexture tex) : _walkSpeed(walkSpeed),
			   _runSpeed(runSpeed) {
		_position = glm::vec2(xPos, yPos);

		_sprite = new Sprite(&_position, tex, 0/*depth*/, Color(255, 255, 255, 255));

		isRunning = false;

		_collider = new SquareCollider(16, 16, &_position, false);
		CollisionLayers::addCollider(_collider);
		SpriteLayers::getInstance()->addToLayer(2, _sprite);
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
		_lastPosition = _position;
		InputManager* im = InputManager::singleton();
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->up)) {
			//move up
			if (!isRunning) {
				_position += glm::vec2(0, _walkSpeed);
			} else {
				_position += glm::vec2(0, _runSpeed);
			}
		}
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->down)) {
			//move down
			if (!isRunning) {
				_position += glm::vec2(0, -_walkSpeed);
			} else {
				_position += glm::vec2(0, -_runSpeed);
			}
		}
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->left)) {
			//move left
			if (!isRunning) {
				_position += glm::vec2(-_walkSpeed, 0);
			} else {
				_position += glm::vec2(-_runSpeed, 0);
			}
		}
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->right)) {
			//move right
			if (!isRunning) {
				_position += glm::vec2(_walkSpeed, 0);
			} else {
				_position += glm::vec2(_runSpeed, 0);
			}
		}
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->a)) {;
		}
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->b)) {;
		}
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->x)) {;
		}
		if (InputManager::singleton()->isKeyDown(
			InputManager::singleton()->y)) {
		}
	}
	bool Being::beat() {
		checkInput();
		_collider->setPos(_position);
		_collider->setLastPos(_lastPosition);
		CollisionLayers::checkAllCollisionsFor(_collider);
		return true;
	}
}