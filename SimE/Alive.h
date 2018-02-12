#pragma once
#include <glm/glm.hpp>

class Alive {
	// This is any object that has to be updated every frame, even if it doesn't move

public:
	virtual ~Alive() {}
	virtual bool beat() { return false; };
	virtual void onCollide(Alive* other) {  };
	void setPos(glm::vec2 pos) { m_position = pos; }
	glm::vec2 getPos() { return m_position; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_lastPosition;
};