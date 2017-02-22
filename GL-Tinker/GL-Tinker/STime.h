#pragma once
#include <chrono>

typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;
typedef std::chrono::high_resolution_clock hrClock;

class STime
{
public:
	static void InitTime();
	static double GetDoubleDeltaTime() { return deltaTime; }
	static float  GetDeltaTime() { return (float)deltaTime; }
	static double GetCurTime() { return curTime; }
	static void UpdateDeltaTime();

	static float globalTimeDilation;

private:
	static double deltaTime;
	static double curTime;
	static TimePoint lastTimer;
};