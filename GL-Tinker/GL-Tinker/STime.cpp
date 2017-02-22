#include "STime.h"

double STime::curTime = 0.0;
double STime::deltaTime = 1.0 / 60.0;
float STime::globalTimeDilation = 1.0f;
TimePoint STime::lastTimer = hrClock::now();

void STime::InitTime()
{
	lastTimer = std::chrono::high_resolution_clock::now();
}

void STime::UpdateDeltaTime()
{
	auto now = hrClock::now();
	deltaTime = ((std::chrono::duration<double>)(now - lastTimer)).count();
	curTime += deltaTime;
	lastTimer = now;
}
