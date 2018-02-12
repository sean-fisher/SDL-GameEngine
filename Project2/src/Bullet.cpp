#include "stdafx.h"
#include "Bullet.h"
#include <SimE/ResourceManager.h>
#include <SimE/GLTexture.h>
#include <iostream>
#include <SimE/SpriteLayers.h>
#include <SimE/Timer.h>

using namespace SimE;

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifetime) :
	m_direction(dir),
	m_position(pos),
	m_speed(speed),
	m_lifetime((float) lifetime) {
//	std::cout << "Bullet Constructor! " << _direction.x << std::endl;
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	SimE::ColorRGBA8 color(0, 255, 255, 255);
	m_sprite = new Sprite(&m_position, uv, "Assets/Test/fun.png", 0, color);
	SpriteLayers::getInstance()->addToLayer(2, m_sprite);
}


Bullet::~Bullet() {
	SpriteLayers::getInstance()->removeFromLayer(2, m_sprite);
	delete m_sprite;
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
	m_position += m_direction * m_speed * Timer::delta();
	if (m_lifetime -= Timer::delta() >= 0) return true;
	return false;
	
}