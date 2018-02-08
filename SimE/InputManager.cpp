#include "stdafx.h"
#include "InputManager.h"
#include <glm/glm.hpp>

namespace SimE {
	InputManager::InputManager() : _mouseCoords(0) {
	}


	InputManager::~InputManager() {
	}
	void InputManager::pressKey(KEYID keyID) {
		_keyMap[keyID] = true;
	}
	void InputManager::releaseKey(KEYID keyID) {
		_keyMap[keyID] = false;
	}
	bool InputManager::isKeyDown(KEYID keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		}
		return false;
	}
	bool InputManager::isKeyPressed(KEYID keyID) {
		return (isKeyDown(keyID) && !wasKeyDown(keyID));
	}

	bool InputManager::wasKeyDown(KEYID keyID) {
		auto it = _prevKeyMap.find(keyID);
		if (it != _prevKeyMap.end()) {
			return it->second;
		}
		return false;
	}
	void InputManager::update() {
		// set previous keys to current keys
		for (auto& it : _keyMap) {
			_prevKeyMap[it.first] = it.second;
		}
	}
	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
}