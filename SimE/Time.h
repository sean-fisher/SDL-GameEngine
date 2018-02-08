#pragma once

namespace SimE {
	class Time {
	public:
		static float getTime() { return TIME; }
		static void setTime(float time) { TIME = time; }
		static float delta() { return _delta; }
		static void setDelta(float delta);
	private:
		static float Time::TIME;
		static float Time::_delta;

	};
	class FPSLimiter {
	public:
		FPSLimiter();
		void init();

		void setMaxFPS(int maxFPS);
		void begin(); // begin frame

		// returns FPS
		float end();
	private:
		void calculateFPS();

		float _fps;
		int _maxFPS;
		unsigned int _frameTime;

		unsigned int _startTicks;
	};
}