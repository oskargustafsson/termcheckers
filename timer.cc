/*
 * timer.cc
 */
#include <ctime>
#include "timer.h"

namespace timer {

	Timer::Timer(int t) : max(t) {}

	Timer::~Timer() {}

	void Timer::startTimer() {
		start = clock();
	}

	int Timer::stopTimer() {
		stop = clock();
		return stop-start;
	}

	int Timer::getTime() {
		int tmp(clock());
		return tmp - start;
	}

	int Timer::getMaxTime(int used)
	{
		if(max-used > max/3)
			return max/30;
		else if(max-used > max/5)
			return max/50;
		else
			return max/300;
	}
}
