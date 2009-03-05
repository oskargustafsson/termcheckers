/*
 * timer.cc
 */
#include <ctime>
#include "timer.h"

namespace timer {

	Timer::Timer(int t) : max(t), used(0) {}

	Timer::~Timer() {}

	void Timer::startTimer() {
		start = clock();
	}

	int Timer::stopTimer() {
		stop = clock();
		used += stop-start;
		return stop-start;
	}

	int Timer::getTime() {
		int tmp(clock());
		return tmp - start;
	}

	int Timer::getTotalTime()
	{
		return used;
	}

	int Timer::getMaxTime()
	{
		if(max-used > max/3)
			return max/30;
		else if(max-used > max/5)
			return max/50;
		else
			return max/300;
	}
}
