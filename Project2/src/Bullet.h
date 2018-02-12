#pragma once
#include <SimE/SpriteBatch.h>
#include <SimE/Sprite.h>
#include <SimE/Alive.h>

class Bullet : public Alive {
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime);
	~Bullet();

	bool beat() override;

	SimE::Sprite* m_sprite;

private:
	float m_lifetime;
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
};

