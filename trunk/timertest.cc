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
  int prevTemp(0);

  while(tmp/100000 < 4) {
    if(prevTemp < tmp/100000) {
      prevTemp = tmp/100000;
      cout << prevTemp << endl;
      cout << "\033[1A";
    }
    tmp = t.getTime();
  }

  tmp = t.stopTimer();
  cout << tmp/100000 << endl;
}


