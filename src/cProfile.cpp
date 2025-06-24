/*
	originated from: cplus.about.com/od/howtodothingsin1/a/timing.htm
*/

#include "cProfile.h"
using namespace std;

cProfile::cProfile()
{
	t_start = 0;
	t_stop = 0;	
	freq = query();
}

void cProfile::start()
{
	t_start = query();
}

void cProfile::stop()
{
	t_stop = query();
}

double cProfile::restart()
{
	stop();
	double t = get();
	start();
	return(t);
}

// take a note that print() restarts the timer
void cProfile::print()
{
	COUT_SET
	cout << this;
}

void cProfile::print(string str)
{
	cout << str << ", " << *this;
}

ostream& operator<<(std::ostream& ostr, cProfile& b)
{
	OSTR_SET
	ostr << "cProfile: " << ": " << b.restart() << "s" << " ==--" << endl;
	return ostr;
}

double cProfile::get()
{
	return tics2secs(t_stop - t_start);
}

ulint cProfile::query()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return((ulint)t.QuadPart);
}


double cProfile::tics2secs(ulint t)
{
	return ((double)t/(double)freq);
}
