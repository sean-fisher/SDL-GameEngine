#include "stdafx.h"
#include "Structs.h"
#include "SpriteLayers.h"

#include <SDL/SDL.h>
#include <iostream>

#define DEBUG_MODE

namespace SimE {
	SquareCollider::SquareCollider() {

	}
	SquareCollider::SquareCollider(int width, int height, glm::vec2 * position, bool isStatic) {
		setPos(*position);
		setWidth(width);
		setHeight(height);
		setStatic(isStatic);

#ifdef DEBUG_MODE
		if (isStatic) {
/*			SpriteLayers::getInstance()->addToLayer(1, new Sprite(position, glm::vec2(.5f, .5f), "", 0, Color(255, 255, 255, 255)));*/
/*			SpriteLayers::getInstance()->addToLayer(1, 
				new Sprite(&glm::vec2(position->x + width / 2, 
									  position->y + height / 2 + 200), 
									  glm::vec2(.5f, .5f), 
									  "", 
									  0.0, 
									  Color(255, 255, 255, 255), 
									  glm::vec2(.5f, .5f)));
			SpriteLayers::getInstance()->addToLayer(1, 
				new Sprite(&glm::vec2(position->x - width / 2, position->y + height / 2), glm::vec2(.5f, .5f), "", 0, Color(255, 255, 255, 255), glm::vec2(.5f, .5f)));
			SpriteLayers::getInstance()->addToLayer(1, new Sprite(&glm::vec2(position->x - width / 2, position->y - height / 2), glm::vec2(.5f, .5f), "", 0, Color(255, 255, 255, 255), glm::vec2(.5f, .5f)));
			SpriteLayers::getInstance()->addToLayer(1, new Sprite(&glm::vec2(position->x + width / 2, position->y - height / 2), glm::vec2(.5f, .5f), "", 0, Color(255, 255, 255, 255), glm::vec2(.5f, .5f)));*/
		}
#endif
	}

	bool SquareCollider::testCollision(Collider* other) {

		float dx = abs(other->getPos().x - this->getPos().x);
		float dy = abs(other->getPos().y - this->getPos().y);

		float widthSum = this->getWidth() / 2 + other->getWidth() / 2;
		float heightSum = this->getHeight() / 2 + other->getHeight() / 2;

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

				if (diffX > diffY) {
					printf("Collision!\n");
					// move other in the X
					if (getPos().x > other->getPos().x) {
						// other must move left
						//getPos()->y -= getHeight() - dy + 16;
						setPos(getLastPos());
					} else {
						// other must move right
						//getPos()->x += getWidth() - dx;
					}

				} else {
					// move other in the Y
					if (getPos().y > other->getPos().y) {
						// other must move down
						//getPos()->y -= heightSum - dy;
					} else {
						// other must move up
						//getPos()->y += heightSum - dy;
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