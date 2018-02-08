#pragma once
#include "stdafx.h"
#include "Time.h"
#include <SDL/SDL.h> 
#include <iostream>

namespace SimE {

	float Time::TIME = 0;
	float Time::_delta = 0;
	void Time::setDelta(float delta) {
		_delta = delta;
	}

	FPSLimiter::FPSLimiter() {

	}
	void FPSLimiter::init() {
		setMaxFPS(1000);
	}
	void FPSLimiter::setMaxFPS(int maxFPS) {
		_maxFPS = maxFPS;
	}
	void FPSLimiter::begin() {
		_startTicks = SDL_GetTicks();
	}
	float FPSLimiter::end() {
		calculateFPS();
		// limit the FPS to _maxFPS;
		Uint32 frameTicks = (Uint32)(SDL_GetTicks()) - _startTicks;

		if (1000.0f /*1000 ms in 1 s*/ / _maxFPS > frameTicks) {
			SDL_Delay((Uint32)(1000.0f / _maxFPS - frameTicks));
		}

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 200) {
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
		return _fps;
	}
	void FPSLimiter::calculateFPS() {

		static const int NUM_SAMPLES = 1000;
		static Uint32 frameTimes[NUM_SAMPLES];
		static int currFrame = 0;

		static Uint32 prevTicks = SDL_GetTicks();
		Uint32 currentTicks = SDL_GetTicks();


		_frameTime = (currentTicks - prevTicks);
		frameTimes[currFrame % NUM_SAMPLES] = (Uint32) _frameTime;

		prevTicks = currentTicks;

		int count;

		currFrame++;
		if (currFrame < NUM_SAMPLES) {
			count = currFrame;
		} else {
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;
		if (frameTimeAverage > 0) {
			_fps = 1000.0f / frameTimeAverage;
		} else {
			_fps = 60.0f;
		}
		

	}
}