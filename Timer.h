#pragma once

#include <chrono>

using namespace std::chrono;

class Timer {
private:
	time_point<high_resolution_clock> start;
public:
	Timer(void);
	double getTimeElapsed(void);
};
