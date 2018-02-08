#pragma once
#include <vector>
#include "Alive.h"

namespace SimE {
	class AliveObjects {
	public:
		AliveObjects();
		~AliveObjects();
		static AliveObjects* getInstance() {
			static AliveObjects singleton;
			return &singleton;
		}
		void birth(Alive* newAlive) {
			_aliveObjects.emplace_back(newAlive);
		}
		bool kill(Alive* alive);
		void killAll();
		void allBeat();
	private:
		std::vector<Alive*> _aliveObjects;
	};

}