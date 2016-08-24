#ifndef STOPWATCH_H_DEFINED_
#define STOPWATCH_H_DEFINED_

#include <vector>
#include <ctime>


class Stopwatch
{
	clock_t start_time;
	bool running;
	double elapsed_time;

public:
	std::vector<double> lap_times;
	Stopwatch();
	void start();
	void stop();
	void reset();
	void lap();
	double elapsed() const;
	bool is_running() const;
};

#endif