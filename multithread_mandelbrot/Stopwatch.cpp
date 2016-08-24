#include "Stopwatch.h"

Stopwatch::Stopwatch() : start_time(0), running(false), lap_times({}), elapsed_time(0.0) {}

void Stopwatch::start()
{
	if (!running)
	{
		running = true;
		start_time = clock();
	}
}

void Stopwatch::stop()
{
	if (running)
	{
		clock_t stop_time = clock();
		running = false;
		elapsed_time += static_cast<double>((stop_time - start_time)) / CLOCKS_PER_SEC;
	}
}

double Stopwatch::elapsed() const
{
	if (running)
	{
		clock_t current_time = clock();
		return elapsed_time + static_cast<double>((current_time - start_time)) / CLOCKS_PER_SEC;
	}
	else
		return elapsed_time;
}

bool Stopwatch::is_running() const
{
	return running;
}

void Stopwatch::lap()
{
	if (lap_times.size() < 1)
	{
		lap_times.push_back(elapsed());
	}
	else
	{
		lap_times.push_back(elapsed() - lap_times.back());
	}
}