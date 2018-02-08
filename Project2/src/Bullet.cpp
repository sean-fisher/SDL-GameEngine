#include "stdafx.h"
#include "Bullet.h"
#include <SimE/ResourceManager.h>
#include <SimE/GLTexture.h>
#include <iostream>
#include <SimE/SpriteLayers.h>
#include <SimE/Time.h>

using namespace SimE;

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime) :
	_direction(dir),
	_position(pos),
	_speed(speed),
	_lifetime((float) lifetime) {
//	std::cout << "Bullet Constructor! " << _direction.x << std::endl;
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	SimE::Color color(0, 255, 255, 255);
	_sprite = new Sprite(&_position, uv, "Assets/Test/fun.png", 0, color);
	SpriteLayers::getInstance()->addToLayer(2, _sprite);
}


Bullet::~Bullet() {
	SpriteLayers::getInstance()->removeFromLayer(2, _sprite);
	delete _sprite;
	std::cout << "Destroy!" << std::endl;
}
/*
void Bullet::draw(SimE::SpriteBatch& batch) {

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	static GLTexture texture = SimE::ResourceManager::getTexture("Assets/Sprites/Characters/Cop/idle_front.png");
	SimE::Color color(255, 255, 255, 255);
	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);

	batch.draw(posAndSize, uv, texture.id, 0.0f, color);
}*/

bool Bullet::beat() {
	_position += _direction * _speed * Time::delta();
	if (_lifetime -= Time::delta() >= 0) return true;
	return false;
	
}