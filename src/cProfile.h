// taken from: cplus.about.com/od/howtodothingsin1/a/timing.htm

#ifndef _cProfile
#define _cProfile
#include "core_defs.h"

#define TIME_PROFILE_START hr_timer timer;timer.startTimer();
#define TIME_PROFILE_END timer.stopTimer(); cout << "---== " << __FUNCTION__ << "(), line " << __LINE__ << ": " << timer.getElapsedTime() << "s" << " ==---" << endl;
#define TIME_PROFILE_RESTART(msg) cout << msg << ": "; TIME_PROFILE_END; timer.startTimer();

class cProfile
{
	public:
		cProfile();

		void start();
		void stop();
		ulint query();
		double get();
		double restart();
		void print();
		void print(std::string);
		friend std::ostream& operator<<(std::ostream&, cProfile&);

	private:
		ulint t_start, t_stop, freq;
		double tics2secs(ulint);
};

#endif