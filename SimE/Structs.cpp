#include "stdafx.h"
#include "Structs.h"
#include "SpriteLayers.h"

#include <SDL/SDL.h>
#include <iostream>

//#define DEBUG_MODE
// if you are moving vertically and hit the corner of a square collider, 
// you will be pushed to the right. If this is not defined, the horizontal 
// equivalent will be true. Both cannot be possible right now because the 
// player gets stuck on a corner.
#define SLIDE_VERTICAL

namespace SimE {
	SquareCollider::SquareCollider() {

	}
	SquareCollider::SquareCollider(int width, int height, glm::vec2 * position, bool isStatic) {
		setPos(*position);
		setWidth(width);
		setHeight(height);
		setStatic(isStatic);

	}

	bool SquareCollider::testCollision(Collider* other) {

		float dx = abs(other->getPos().x - this->getPos().x);
		float dy = abs(other->getPos().y - this->getPos().y);

		int widthSum = (int)(this->getWidth() / 2.0f + other->getWidth() / 2.0f);
		int heightSum = (int)(this->getHeight() / 2.0f + other->getHeight() / 2.0f);

		if (dx < widthSum && dy < heightSum) {
			// Collision!
			if (this->isStatic()) {
				// we can assume the other collider is not static.
				// we move the other out of the way.
				

			} else if (other->isStatic()) {
				// we know this one is the only that moves,
				// so we move this out of the way

				float diffY = heightSum - dy;
				float diffX = widthSum - dx;

				if (m_alive && other->getAlive()) {
					m_alive->onCollide(other->getAlive());
				}

				if (diffX > diffY) {
					// move this in the X
					if (m_position.x > other->getPos().x) {
						//  ___|_
						// |	|
						// |	|
						// |____|
						//     |----this

						// if close to the right side, push this right
						int roundedEdgeSize = 2;
						int distToEdge = (int)(-m_position.x + other->getPos().x + other->getWidth() / 2);
						
						int displace;
						if (other->getPos().y < m_position.y) {
							displace = (int)(other->getPos().y + other->getHeight() / 2.0f + m_height / 2.0f);
						} else {
							displace = (int)(other->getPos().y - other->getHeight() / 2.0f - m_height / 2.0f);
						}
						
						if (distToEdge < roundedEdgeSize) {
							setPos(glm::vec2(m_position.x + 1, displace));
						} else {
							setPos(glm::vec2(m_position.x, displace));
						}
					} else {
						//  _|___
						// |	|
						// |	|
						// |____|
						//  |------this

						// if close to the left side, push this left
						int roundedEdgeSize = 2;
						int distToEdge = (int)(m_position.x - other->getPos().x + other->getWidth() / 2);
						int displace;
						if (other->getPos().y < m_position.y) {
							displace = (int)(other->getPos().y + other->getHeight() / 2.0f + m_height / 2.0f);
						} else {
							displace = (int)(other->getPos().y - other->getHeight() / 2.0f - m_height / 2.0f);
						}
						if (distToEdge < roundedEdgeSize) {
							setPos(glm::vec2(m_position.x - 1, displace));
						} else {
							setPos(glm::vec2(m_position.x, displace));
						}
					}

				} else {
					// move this in the Y
					if (m_position.y > other->getPos().y) {
						//		 ____
						// this-|	 |-this
						//		|	 |
						//		|____|

#ifndef SLIDE_VERTICAL
						// if close to the top side, push this up
						int roundedEdgeSize = 2;
						int distToEdge = -m_position.y + other->getPos().y + other->getHeight() / 2;
						if (distToEdge < roundedEdgeSize) {
							setPos(glm::vec2(m_lastPosition.x, m_position.y + 1));
						} else {
							setPos(glm::vec2(m_lastPosition.x, m_position.y));
						}
#else
						setPos(glm::vec2(m_lastPosition.x, m_position.y));
#endif
					} else {
						//      ____
						//     |	|
						//     |	|
						//this-|____|-this
						//

#ifndef SLIDE_VERTICAL
						// if close to the bottom side, push this down
						int roundedEdgeSize = 2;
						int distToEdge = m_position.y - other->getPos().y + other->getHeight() / 2;
						if (distToEdge < roundedEdgeSize) {
							setPos(glm::vec2(m_lastPosition.x, m_position.y - 1));
						} else {
							setPos(glm::vec2(m_lastPosition.x, m_position.y));
						}
#else
						setPos(glm::vec2(m_lastPosition.x, m_position.y));
#endif
					}
				}
			} else {
				// both are non-static, so we move both
			}
		}

		return false;
	}
	void CollisionLayers::checkAllCollisionsFor(Collider* c) {
		CollisionLayers* sCL = singleton();
		for (size_t i = 0; i < sCL->colliders_nonstatic.size(); i++) {
			if (c != sCL->colliders_nonstatic[i]) {
				c->testCollision(sCL->colliders_nonstatic[i]);
			}
		}
		for (size_t i = 0; i < sCL->colliders_static.size(); i++) {
			if (c != sCL->colliders_static[i]) {
				c->testCollision(sCL->colliders_static[i]);
			}
		}
	}
	void CollisionLayers::addCollider(Collider* c) {
		CollisionLayers* sCL = singleton();
		if (c->isStatic()) {
			sCL->colliders_static.push_back(c);
		} else {
			sCL->colliders_nonstatic.push_back(c);
		}
	}
}