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
#include <SimE/Timer.h>
#include <SimE/Alive.h>
#include <SimE/SpriteFont.h>

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
	
	float m_fps;
	
	SimE::Window _window;
	int m_screenWidth;
	int m_screenHeight;
	
	GameState m_currGameState;


	SimE::GLSLProgram m_colorProgram;
	SimE::FPSLimiter m_fpsLimiter;

	SimE::Camera2D m_cam;
	SimE::Camera2D m_uiCam;
	SimE::SpriteBatch m_spriteBatch;
	SimE::InputManager m_inputManager;


	std::vector<Bullet> m_bullets;

	std::vector<Alive*> m_aliveObjects;

};

