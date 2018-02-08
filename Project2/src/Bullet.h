#pragma once
#include <SimE/SpriteBatch.h>
#include <SimE/Sprite.h>
#include <SimE/Alive.h>

class Bullet : public Alive {
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime);
	~Bullet();

	bool beat() override;

	SimE::Sprite* _sprite;

private:
	float _lifetime;
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
};

