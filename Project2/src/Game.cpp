#pragma once
#include "stdafx.h"
#include "Game.h"

#include <iostream>
#include <string>
#include <SimE/SDLerrors.h>
#include <SimE/Window.h>
#include <SimE/GLSLProgram.h>
#include <SimE/SimE_class.h>
#include <SimE/ResourceManager.h>
#include <SimE/SpriteLayers.h>
#include <SimE/AliveObjects.h>
#include <algorithm>
#include <SimE/Timer.h>
#include <SimE/TileMap.h>
#include <SimE/Being.h>
#include <SimE/SpriteFont.h>
#include <SimE/Timer.h>

// Help find memory leaks
#include <stdlib.h>  
#include <crtdbg.h>


using namespace SimE;

Game::Game()
{
    m_screenWidth = 640;
    m_screenHeight = 480;
    SimE::Timer::setTime(0);
	//SimE:Time::delta = 0;
    m_currGameState = GameState::PLAY;

    m_cam.init(m_screenWidth, m_screenHeight);
    
}

Game::~Game()
{
    
}

void Game::run() {
    initSystems();

	// load initial stuff
	glm::vec2 pos(0, 0);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	SimE::ColorRGBA8 color(255, 255, 255, 255);

	static GLTexture texture = SimE::ResourceManager::getTexture("Assets/Sprites/Characters/Cop/idle_front.png");
	Being player = Being(.5f, 1, 0, 0, texture);
	player.cam = &m_cam;
	SimE::AliveObjects::getInstance()->birth(&player);
	//SpriteLayers::getInstance()->addToLayer(GROUND, new Sprite(&pos, uv, texture, 0, color));
	//_spriteBatch.addSprite();

	// load map; currently this only loads one layer
	//TileMap tm = TileMap("Assets/Tilesets/TestMapCSV.csv");
	//TileMap tm = TileMap("Assets/Tilemaps/DecorationsMapTest_Tile Layer 1.csv");
	
	Being testTile = Being(0, 0, 50, 50, SimE::ResourceManager::getTexture("Assets/Test/fun.png"));
	glm::vec2 pos2 = glm::vec2(50, 50);
	SquareCollider col = SquareCollider(64, 64, &pos2, true);
	col.setAlive(&testTile);


	CollisionLayers::addCollider(&col);
	TileMap tm = TileMap("Assets/Tilemaps/", "DecorationsMapTest");
	Tileset ts = Tileset("Assets/Tilesets/Decorations1.tsx");
	SpriteLayers::getInstance()->addMapToBatch(&tm, &ts);
	// draw map


    gameLoop();
    _CrtDumpMemoryLeaks();  
}

void Game::initSystems() {

    SimE::init();
	unsigned int flags = 0;
	//unsigned int flags = FULLSCREEN;
    _window.create("Game Engine", m_screenWidth, m_screenHeight, flags);

	// Set up controls
	InputManager::singleton()->up = SDLK_UP;
	InputManager::singleton()->down = SDLK_DOWN;
	InputManager::singleton()->left = SDLK_LEFT;
	InputManager::singleton()->right = SDLK_RIGHT;
	InputManager::singleton()->a = SDLK_d;
	InputManager::singleton()->b = SDLK_s;
	InputManager::singleton()->x = SDLK_w;
	InputManager::singleton()->y = SDLK_a;
	InputManager* im = InputManager::singleton();

	// set the number of sprite layers
	SpriteLayers::getInstance()->init(3);

}


void Game::processInput() {
    SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_currGameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN:
			InputManager::singleton()->pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			InputManager::singleton()->releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			InputManager::singleton()->pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			InputManager::singleton()->releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			InputManager::singleton()->setMouseCoords((float) evnt.motion.x, (float) evnt.motion.y);
			break;
		}
		
	}

	if (InputManager::singleton()->isKeyDown(SDLK_i)) {
		//move _cam up
		m_cam.setPosition(m_cam.getPosition() + glm::vec2(0, m_cam.CAMERA_SPEED * Timer::delta()));
	}
	if (InputManager::singleton()->isKeyDown(SDLK_l)) {
		//move _cam left
		m_cam.setPosition(m_cam.getPosition() + glm::vec2(m_cam.CAMERA_SPEED * Timer::delta(), 0));
	}
	if (InputManager::singleton()->isKeyDown(SDLK_k)) {

		//move _cam down
		m_cam.setPosition(m_cam.getPosition() + glm::vec2(0, -m_cam.CAMERA_SPEED * Timer::delta()));
	}
	if (InputManager::singleton()->isKeyDown(SDLK_j)) {

		//move _cam right
		m_cam.setPosition(m_cam.getPosition() + glm::vec2(-m_cam.CAMERA_SPEED * Timer::delta(), 0));
	}
	if (InputManager::singleton()->isKeyDown(SDLK_q)) {

		//move _cam back
		m_cam.setScale(m_cam.getScale() + m_cam.SCALE_SPEED);
	}
	if (InputManager::singleton()->isKeyDown(SDLK_e)) {

		//move _cam forward
		m_cam.setScale(m_cam.getScale() - m_cam.SCALE_SPEED);
	}
	
	// Shoot a bullet on left click
	if (InputManager::singleton()->isKeyDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = InputManager::singleton()->getMouseCoords();
		mouseCoords = m_cam.screenToWorldCoor(mouseCoords);

		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);
		//_bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
		SimE::AliveObjects::getInstance()->birth(new Bullet(playerPosition, direction, .5f, 500));
	}
}

void Game::gameLoop() {
	float maxFps = 500;
	m_fpsLimiter.setMaxFPS((int) maxFps);

	const float ms_per_second = 1000;
	const float target_fps = ms_per_second / maxFps;
	const  int max_physics_steps = 6;
	const float max_delta_time = 1.0f;
	float prevTicks = (float) SDL_GetTicks();

    while(m_currGameState != GameState::EXIT) {

		// calculate the amount of time between frames
		float newTicks = (float) SDL_GetTicks();
		float frameTime = newTicks - prevTicks;
		prevTicks = newTicks;

		float totalDeltaTime = frameTime / target_fps;

		m_fpsLimiter.begin();
		Uint16 startbeats = (Uint16) SDL_GetTicks();
	
		m_inputManager.update();
		processInput();
		m_cam.update();

		//int i = 0;
		//float deltaTime = std::min(totalDeltaTime, max_delta_time);
		/*while (totalDeltaTime > 0.0f && i < max_physics_steps) {
			deltaTime = std::min(totalDeltaTime, max_delta_time);

			// do physics stuff if necessary
			totalDeltaTime -= deltaTime;
			i++;
		}*/
		SimE::Timer::setDelta(frameTime);
		SimE::Timer::setTime(SimE::Timer::getTime() + .1f);

		

		// TODO: call all beats 
		AliveObjects::getInstance()->allBeat();

		// draw everything
		SpriteLayers::getInstance()->beginDrawing();
		SpriteLayers::getInstance()->drawAll(&m_cam, &_window);
		SpriteLayers::getInstance()->endDrawing(&_window);


		// draw the bullets
		/*for (size_t i = 0; i < _bullets.size(); i++) {
			_bullets[i].draw(_spriteBatch);
		}*/
		//drawGame();

		// Swap our buffer and draw everything to the screen
		_window.swapBuffer();

		m_fps = m_fpsLimiter.end();


    }
}

/*void Game:: drawGame() {
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// enable the shader
    m_colorProgram.use();
    glActiveTexture(GL_TEXTURE0);

    GLint textureLocation = m_colorProgram.getUniformLocation("spriteSampler");
    glUniform1i(textureLocation, 0);

    GLuint timeLoc = m_colorProgram.getUniformLocation("time");
    // send variable to shader
    glUniform1f(timeLoc, 0);

    // Set the ortho camera matrix
    GLuint projLoc = m_colorProgram.getUniformLocation("orthoProj");
    glm::mat4 camMatrix = m_cam.getCameraMatrix();

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &camMatrix[0][0]);
	
	m_spriteBatch.begin(); // 3 is GlyphSortType::TEXTURE
	

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	static GLTexture texture = SimE::ResourceManager::getTexture("Assets/Sprites/Characters/Cop/idle_front.png");

	SimE::ColorRGBA8 color(255, 255, 255, 255);

	
	m_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
	m_spriteBatch.draw(pos + glm::vec4(50, 0, 0, 0), uv, texture.id, 0.0f, color);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

    // unbind the shader
    glBindTexture(GL_TEXTURE_2D, 0);

	// disable the shader
    m_colorProgram.unuse();
}*/

