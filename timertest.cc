/*
 * timertest.cc
 */
#include <iostream>
#include "timer.h"

using namespace std;
using namespace timer;

int main() {
  Timer t = Timer::Timer();
  t.startTimer();
  int tmp = t.getTime();

  while(tmp/100000 < 4) {
    cout << tmp/100000 << endl;
    tmp = t.getTime();
  }

  tmp = t.stopTimer();
  cout << tmp/100000 << endl;
}


