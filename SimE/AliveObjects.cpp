#include "AliveObjects.h"
#include "stdafx.h"
#include <iostream>

namespace SimE {
	AliveObjects::AliveObjects() {
	}


	AliveObjects::~AliveObjects() {
	}
	bool AliveObjects::kill(Alive * alive) {
		return false;
	}
	void AliveObjects::killAll() {
	}
	void AliveObjects::allBeat() {
		for (size_t i = 0; i < _aliveObjects.size(); i++) {
			bool isAlive = _aliveObjects[i]->beat();
			if (!isAlive) {
				Alive* dead = _aliveObjects[i];
				_aliveObjects.erase(_aliveObjects.begin() + i);
				delete dead;
			}
		}
	}
}