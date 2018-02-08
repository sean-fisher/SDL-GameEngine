#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <SimE/sprite.h>
#include <SimE/GLSLProgram.h>
#include <SimE/GLTexture.h>
#include <SimE/Window.h>
#include <SimE/Camera2D.h>
#include <SimE/Vertex.h>
#include <SimE/SpriteBatch.h>
#include <SimE/InputManager.h>
#include "Bullet.h"
#include <SimE/Time.h>
#include <SimE/Alive.h>

enum class GameState { PLAY, EXIT };

class Game
{
public:
	Game();
	~Game();

	void run();



private:
	
	// initializes SDL and OpenGL
	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	
	float _fps;
	
	SimE::Window _window;
	int _screenWidth;
	int _screenHeight;
	
	GameState _currGameState;


	SimE::GLSLProgram _colorProgram;
	SimE::FPSLimiter _fpsLimiter;

	SimE::Camera2D _cam;
	SimE::SpriteBatch _spriteBatch;
	SimE::InputManager _inputManager;

	std::vector<Bullet> _bullets;

	std::vector<Alive*> _aliveObjects;
};

