#include "Timer.h"

Timer::Timer(void) : start(high_resolution_clock::now()) {}

double Timer::getTimeElapsed(void) {
	time_point<high_resolution_clock> now = high_resolution_clock::now();
	duration<double, std::milli> elapsed(now - start);
	start = high_resolution_clock::now();
	return elapsed.count();
}