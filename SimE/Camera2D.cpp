#include "stdafx.h"
#include "Camera2D.h"
#include <glm\glm.hpp>
#include <iostream>

namespace SimE {
	Camera2D::Camera2D() : _position(0, 0), _velocity(0, 0), _cameraMatrix(1.0f), _scale(1.0f), _needsMatrixUpdate(true), _screenWidth(640), _screenHeight(480) {

	}


	Camera2D::~Camera2D() {
	}

	void Camera2D::update() {
		if (_needsMatrixUpdate) {
			//translation
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			//scale
			glm::vec3 scale(_scale, _scale, 0);
			_cameraMatrix = glm::scale(glm::mat4(1), scale) * _cameraMatrix;
			_needsMatrixUpdate = false;
		}
		if (_needsMoveUpdate) {

		}
	}
	void Camera2D::init(int screenWidth, int screenHeight) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

	}
	bool Camera2D::canBeSeen(const glm::vec2 & position, const glm::vec2 dimensions) {

		glm::vec2 scaledScreen = glm::vec2(_screenWidth, _screenHeight) / (_scale);

		const float min_distance_x = (dimensions.x + scaledScreen.x) / 2.0f;
		const float min_distance_y = (dimensions.y + scaledScreen.y) / 2.0f;

		glm::vec2 centerPos = position + dimensions / 2.0f;
		glm::vec2 centerCamPos = _position;
		glm::vec2 distVec = centerPos- centerCamPos;

		float xDepth = min_distance_x - abs(distVec.x);
		float yDepth = min_distance_y - abs(distVec.y);

		if (xDepth > 0 && yDepth > 0) {
			// collision
			return true;
		}
		return false;
	}
	glm::vec2 Camera2D::screenToWorldCoor(glm::vec2 screenCoords) {
		// invert y axis
		screenCoords.y = _screenHeight - screenCoords.y;

		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2); // put zero in the center
		screenCoords /= _scale;
		// translate
		screenCoords += _position;
		return screenCoords;
	}
}