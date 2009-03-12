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
			Timer(int);
			~Timer();

			/* retreiving the time variables */
			int getTime(); //mellantid
			int getUsedTime();

			int getMaxTime(int);
			int getTotalTime();

			/*starting and stopping timer */
			void startTimer();
			int stopTimer(); //returns used time

		private:
			int max, start, stop;

	};
}

#endif /* TIMER_H */
