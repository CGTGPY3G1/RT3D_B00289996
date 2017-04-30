#include "Time.h"
#include <SDL_timer.h>
namespace B00289996 {
	const float Timer::FIXED_DELTA_TIME = 0.02f;
	float Timer::deltaTime = 0.0f;
	unsigned int  Timer::currentTime = 0, Timer::lastFrameTime = 0;

	Timer::~Timer() {
	}

	const float Timer::GetDeltaTime() {
		return deltaTime;
	}

	const float Timer::GetFixedDeltaTime() {
		return FIXED_DELTA_TIME;
	}

	void Timer::Update() {
		currentTime = SDL_GetTicks();
		deltaTime = ((float)(currentTime - lastFrameTime)) / 1000.0f;
		lastFrameTime = currentTime;
	}

	void Timer::Init() {
		lastFrameTime = SDL_GetTicks();
		currentTime = lastFrameTime;
	}
}