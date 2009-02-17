/*
 * timer.cc
 */
#include <ctime>
#include "timer.h"

namespace timer {

  Timer::Timer() {}

  Timer::~Timer() {}

  void Timer::startTimer() {
    starttime = clock();
  }

  int Timer::stopTimer() {
    stoptime = clock();
    return stoptime - starttime;
  }

  int Timer::getTime() {
    int tmp(clock());
    return tmp - starttime;
  }
}
