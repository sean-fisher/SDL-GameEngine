#pragma once
#include "stdafx.h"
#include "Timer.h"
#include <SDL/SDL.h> 
#include <iostream>

namespace SimE {

	float Timer::TIME = 0;
	float Timer::m_delta = 0;
	void Timer::setDelta(float delta) {
		m_delta = delta;
	}

	float FPSLimiter::m_fps = 60;

	FPSLimiter::FPSLimiter() {

	}
	void FPSLimiter::init() {
		setMaxFPS(1000);
	}
	void FPSLimiter::setMaxFPS(int maxFPS) {
		m_maxFPS = maxFPS;
	}
	void FPSLimiter::begin() {
		m_startTicks = SDL_GetTicks();
	}
	float FPSLimiter::end() {
		calculateFPS();
		// limit the FPS to _maxFPS;
		Uint32 frameTicks = (Uint32)(SDL_GetTicks()) - m_startTicks;

		if (1000.0f /*1000 ms in 1 s*/ / m_maxFPS > frameTicks) {
			SDL_Delay((Uint32)(1000.0f / m_maxFPS - frameTicks));
		}

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 200) {
			std::cout << m_fps << std::endl;
			frameCounter = 0;
		}
		return m_fps;
	}
	void FPSLimiter::calculateFPS() {

		static const int NUM_SAMPLES = 1000;
		static Uint32 frameTimes[NUM_SAMPLES];
		static int currFrame = 0;

		static Uint32 prevTicks = SDL_GetTicks();
		Uint32 currentTicks = SDL_GetTicks();


		m_frameTime = (currentTicks - prevTicks);
		frameTimes[currFrame % NUM_SAMPLES] = (Uint32) m_frameTime;

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
			m_fps = 1000.0f / frameTimeAverage;
		} else {
			m_fps = 60.0f;
		}
		

	}
}