#pragma once

namespace SimE {
	class Timer {
	public:
		static float getTime() { return TIME; }
		static void setTime(float time) { TIME = time; }
		static float delta() { return m_delta; }
		static void setDelta(float delta);
	private:
		static float Timer::TIME;
		static float Timer::m_delta;

	};
	class FPSLimiter {
	public:
		FPSLimiter();
		void init();

		void setMaxFPS(int maxFPS);
		static int getFPS() {return m_fps;}
		void begin(); // begin frame

		// returns FPS
		float end();
	private:
		void calculateFPS();

		static float m_fps;
		int m_maxFPS;
		unsigned int m_frameTime;

		unsigned int m_startTicks;
	};
}