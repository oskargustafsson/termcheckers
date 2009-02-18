#ifndef TIMER_H
#define TIMER_H

/*
 * timer.h
 */
#include <ctime>

namespace timer {

  class Timer {

  public:

    /*Constructor and destructor */
    Timer();
    ~Timer();

    /* retreiving the time variables */
    int getTime(); //mellantid

    /*starting and stopping timer */
    void startTimer();
    int stopTimer(); //returns total time

  private:
    int starttime, stoptime;

  };

}

#endif /* TIMER_H */
