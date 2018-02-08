#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace SimE {
	class Camera2D {
	public:
		float CAMERA_SPEED = .3f;
		float SCALE_SPEED = .01f;
		Camera2D();
		~Camera2D();

		void update(); // updates camera matrix if needed
		void init(int screenWidth, int screenHeight);

		bool canBeSeen(const glm::vec2& position, const glm::vec2 dimensions);

		void setPosition(glm::vec2& newPosition) {
			_position = newPosition;
			_needsMatrixUpdate = true;
		}
		void setVelocity(glm::vec2& newVel) {
			_velocity = newVel;
			_needsMatrixUpdate = true;
		}
		void setScale(float newScale) {
			_scale = newScale;
			_needsMatrixUpdate = true;
		}

		glm::vec2 screenToWorldCoor(glm::vec2 screenCoords);

		float getScale() { return _scale; }
		glm::vec2 getPosition() { return _position; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }
		glm::vec2 getVelocity() { return _velocity; }


	private:
		int _screenWidth;
		int _screenHeight;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;

		glm::vec2 _velocity;

		bool _needsMatrixUpdate;
		bool _needsMoveUpdate;
	};

}