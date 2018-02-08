#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace SimE {

	typedef unsigned int KEYID;

	class InputManager {
	public:
		InputManager();
		~InputManager();
		static InputManager* singleton() {
			static InputManager singleton; 
			return &singleton;
		}

		void pressKey(KEYID keyID);
		void releaseKey(KEYID keyID);
		bool isKeyDown(KEYID keyID); // key is down
		bool isKeyPressed(KEYID keyID); // key was just pressed
		bool wasKeyDown(KEYID keyID);

		void update();

		void setMouseCoords(float x, float y);
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

		KEYID up;
		KEYID down;
		KEYID left;
		KEYID right;
		KEYID a;
		KEYID b;
		KEYID x;
		KEYID y;

	private:
		std::unordered_map<KEYID, bool> _keyMap;
		std::unordered_map<KEYID, bool> _prevKeyMap;
		glm::vec2 _mouseCoords;

	};

}