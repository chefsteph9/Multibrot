/*
Stephen Jenks
multibrot.cpp : A file to measure the performance of computing the mandelbrot set using a variable number of threads


sources:
I already had my sources listed in this format before I read that you wanted my sources listed by where
I used them. So I added numbers to each one and put the numbers by where I used the source.

	1.
	http://www.cplusplus.com/doc/tutorial/files/ for writing to files
	
	2.
	http://netpbm.sourceforge.net/doc/pgm.html and
	https://en.wikipedia.org/wiki/Netpbm_format for learning about pgm files
	
	3.
	Professor Ordonez for fixing my skewing pgm files
	
	4.
	https://en.wikipedia.org/wiki/Mandelbrot_set for the pseudocode to calculate the mandelbrot set
	
	5.
	http://www.cplusplus.com/forum/general/103477/ for using int to char to write pure binary to a file
	* No longer used since I changed from using ints to using chars
	
	6.
	http://marknelson.us/2012/05/23/c11-threading-made-easy/
	https://solarianprogrammer.com/2011/12/16/cpp-11-thread-tutorial/ For easy ways to do multithreading in C++11
	
	7.
	https://msdn.microsoft.com/en-us/library/70abkas3.aspx To force compilation when linker thinks I defined my stopwatch functions twice(I didn't)
															EDIT: 2015-11-16. Yeah, I did. 

	8.
	http://stackoverflow.com/questions/11563963/writing-a-binary-file-in-c-very-fast For the fastest method to write to file

	9.
	http://www.cplusplus.com/reference/cstdio/fopen/
	http://www.cplusplus.com/reference/cstdio/fwrite/	For documentation on how to use fopen and fwrite respectively.

	10.
	http://stackoverflow.com/questions/14386/fopen-deprecated-warning To enable fwrite because microsoft apparently thinks fopen is unsafe.

multithread_mandelbrot.cpp: Original work to get multithreaded mandelbrot computation correct.


2015-11-10 work on multithread_mandelbrot.cpp
2015-11-11 Finished multithread_mandelbrot.cpp

2015-11-12	Started work on multibrot.cpp
2015-11-13	Cleaned up the UI, added timer, made writing the output file an option
2015-11-14	General optimizations, made write_to_file a separate function
2015-11-15	Made hi priority toggle
2015-11-16	Fixed helper_funcs.h and cpp to not give a linker error and removed option to force compile when linker is not happy.
			Switched to using a vector<vector<char>> instead of ints to test if the converstion from int to char was slowing down my write times. It didn't help
			Switched to using fwrite() and fopen() instead of using fstreams to write output to file. (literally 100 times faster)
2015-11-17	Increased the maximum number of iterations possible with grayscale from 85 to 255 by taking advantage of the fact that I changed my escape_time function
			to return a char instead of an int.
2015-11-23	Added the formal_tests function
2015-11-30	Minor improvements to zoom and toggle high priority mode features.			

*/

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <thread>
#include <algorithm>
#include <numeric>
#include "helper_funcs.h"
#include "Stopwatch.h"
#include <windows.h>
#include <tchar.h>

using std::vector;
using std::ofstream;
using std::ostream;
using std::cout;
using std::cin;
using std::endl;
using std::thread;
using std::find;
using std::min;
using std::accumulate;


// Function Declarations:
void formal_tests();
void run_test_list(vector<unsigned> test_list);
Stopwatch launch_multibrot_test();
void multithread_mandelbrot(unsigned tid);
void singlethread_mandelbrot();
char mandelbrot_escape_time(double x0, double y0);
Stopwatch write_image_file();
void display_parameters();


// Global Variable initializations: 
vector<vector<char>> array;
double zoom = 1.0;
double x_offset = 0;
double scale, y_start_coord, x_start_coord;
// 4k = 4096*2304
unsigned width = 4096;
unsigned height = 2304;
int iterations = 50;
unsigned num_threads;
bool write = false;
bool grayscale = true;
bool hi_priority = false;
const char* filename = "C:\\Users\\Stephen\\Multibrot\\C++ output\\Multibrot Result.pgm";



int main()
{
	vector<unsigned> default_num_threads_test{ 0, 1, 2, 4, 8, 16 };
	vector<unsigned> custom_num_threads_test = { 32 };
	bool done = false;
	char command;

	cout << "Number of threads vs time to compute the mandelbrot set test utility." << endl
		<< "Developed by Stephen Jenks" << endl << endl
		<< "Welcome, Professor.";
	while (!done)
	{
		cout << endl << "Please enter a command(enter 'H' for a list of commands): ";
		cin >> command;
		switch (command)
		{
		case 'h':
		case 'H':
		{
			cout << endl << "This utility tests how long it takes to compute the mandelbrot set " << endl
				<< "using a variable number of threads. Specifying 0 threads will" << endl
				<< "compute the mandelbrot set using a dedicated single threaded function." << endl
				<< "Specifying 1 or more threads will use a dedicated multithreaded function." << endl
				<< "Enter:" << endl
				<< "\t'P' to display current test parameters." << endl
				<< "\t'R' to change the resolution of the calculation." << endl
				<< "\t'Z' to change the zoom." << endl
				<< "\t'I' to change the max number of iterations in the calculation." << endl
				<< "\t'W' to toggle writing output to file(takes much longer)." << endl
				<< "\t'G' to toggle grayscale or pure black/white output." << endl
				<< "\t'T' to toggle high priority mode." << endl
				<< "\t'M' to modify your custom test list." << endl
				<< "\t'D' to run the default test list." << endl
				<< "\t'C' to run your custom test list." << endl
				<< "\t'F' to run the test used to produce the graph in my report.(Takes several minutes)" << endl
				<< "\t'Q' to quit." << endl;
			break;
		}

		case 'p':
		case 'P':
		{
			display_parameters();
			cout << "Default number of threads to test:" << endl << default_num_threads_test << endl
				<< "Custom number of threads to test:" << endl << custom_num_threads_test << endl << endl;
			break;
		}

		case 'r':
		case 'R':
		{
			cout << "Enter width in pixels: ";
			cin >> width;
			cout << "Enter height in pixels: ";
			cin >> height;
			cout << "The resolution is now " << width << "x" << height << " pixels WxH." << endl;
			break;
		}
		case 'z':
		case 'Z':
		{
			cout << "Enter the new zoom level: ";
			cin >> zoom;
			if (zoom <= 0)
			{
				cout << "Invalid zoom value. Setting zoom to 1" << endl;
				zoom = 1;
			}
			break;
		}

		case 'i':
		case 'I':
		{
			cout << "Enter the new number of iterations (Max of 255 with grayscale): ";
			cin >> iterations;
			cout << "The max number of iterations is now: " << iterations << endl;
			break;
		}

		case 'w':
		case 'W':
		{
			write = !write;
			cout << "Write to file(1/0): " << write << endl;
			break;
		}

		case 'g':
		case 'G':
		{
			grayscale = !grayscale;
			cout << "Grayscale or B/W(1/0): " << grayscale << endl;
			break;
		}

		case 't':
		case 'T':
		{
			DWORD dwPriClass;
			bool success = false;
			if (hi_priority)
			{
				success = SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
				dwPriClass = GetPriorityClass(GetCurrentProcess());
			}
			else
			{
				success = SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
				dwPriClass = GetPriorityClass(GetCurrentProcess());
			}

			if (success)
			{
				cout << "Succeeded " << endl;
				hi_priority = !hi_priority;
			}
			else
			{
				cout << "Failed" << endl;
			}
			
			cout << "High priority(1/0): " << hi_priority << endl;
			break;
		}

		case 'm':
		case 'M':
		{
			// TODO: account for bad user input.
			bool done_modifying_custom_tests = false;
			char modify_custom_tests_command;
			while (!done_modifying_custom_tests)
			{
				cout << endl << "The current custom test list is:" << endl << custom_num_threads_test << endl;
				cout << endl << "Enter: " << endl
					<< "\t'+' to add tests." << endl
					<< "\t'-' to delete tests." << endl
					<< "\t'*' to clear all tests." << endl
					<< "\t'/' to finish modifying test list." << endl
					<< "Enter a command: ";
				cin >> modify_custom_tests_command;
				switch (modify_custom_tests_command)
				{
				case '+':
				{
					int test_to_add = 0;
					cout << "\nEnter thread count to test.\n"
						<< "0 uses a dedicated single threaded algorithm.\n"
						<< "1 or greater uses a multithreaded algorithm.\n"
						<< "\nEnter test to add(negative number quits): ";
					cin >> test_to_add;
					cin.clear();
					cin.ignore(100, '\n');
					while (test_to_add >= 0)
					{
						custom_num_threads_test.push_back(test_to_add);
						cout << "Successfully added " << test_to_add << endl;
						cout << "Enter test to add(negative number quits): ";
						cin >> test_to_add;
						cin.clear();
						cin.ignore(100, '\n');
					}
					break;
				}

				case '-':
				{
					int test_to_delete = 0;
					cout << endl << "Enter test to delete(negative number quits): ";
					cin >> test_to_delete;
					cin.ignore(100, '\n');
					while (test_to_delete >= 0)
					{
						auto it = find(custom_num_threads_test.begin(), custom_num_threads_test.end(), test_to_delete);
						if (it != custom_num_threads_test.end())
						{
							custom_num_threads_test.erase(it);
							cout << "Successfully deleted " << test_to_delete;
						}
						else
						{
							cout << test_to_delete << " Not found in test list." << endl;
						}
						cout << endl << "Enter test to delete(negative number quits): ";
						cin >> test_to_delete;
						cin.ignore(100, '\n');
					}
					break;
				}

				case '*':
					custom_num_threads_test.clear();
					cout << "Test list cleared." << endl;
					break;

				case '/':
					done_modifying_custom_tests = true;
					break;

				default:
					cin.ignore(100, '\n');
					cout << "Unknown command :\\" << endl;
					cin.clear();
					break;
				}
			}
			break;
		}

		case 'd':
		case 'D':
		{
			run_test_list(default_num_threads_test);
			break;
		}

		case 'c':
		case 'C':
		{
			run_test_list(custom_num_threads_test);
			break;
		}

		case 'f':
		case 'F':
		{
			formal_tests();
			break;
		}

		case 'q':
		case 'Q':
			cin.ignore(100, '\n');
			cout << "Goodbye.\n";
			done = true;
			break;

		default:
			cin.ignore(100, '\n');
			cin.clear();
			cout << "Unknown command :\\";
			
			
		}
	}
}

/*
This is what I used to gather the data for the graph of threads vs time on my report.
Computes the mandelbrot set 25 times for each number of threads and computes the average
time it took.
*/
void formal_tests()
{
	unsigned wtemp = width;
	unsigned htemp = height;

	// I hard set the resolution for the test at 16k x 9k because on my machine the fastest it could compute this
	// was about 1 second and I wanted that as a base.
	width = 16000;
	height = 9000;

	vector<unsigned> test_list = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 20, 24, 28, 32 };

	for (auto thread_count : test_list)
	{
		vector<double> times(25, 0);
		for (unsigned i = 0; i < 25; i++)
		{
			num_threads = thread_count;
			Stopwatch timer = launch_multibrot_test();
			times[i] = timer.elapsed();
		}

		double average = accumulate(times.begin(), times.end(), 0.0) / times.size();
		cout << "Average time with " << thread_count << " threads: " << average << " seconds." << endl;
	}

	width = wtemp;
	height = htemp;
}

/* Given a vector containing a list of thread counts to test this function:
	
	Outputs the parameters of the test.
	Makes sure there are no invalid test parameters.
	For each element in the test list runs launch_multibrot_test() with the specified number of threads
	Displays the time it took to compute the mandelbrot set with the specified number of threads.
	If write was enabled writes the output to a file after all the tests are completed.
*/
void run_test_list(vector<unsigned> test_list)
{
	display_parameters();
	if (grayscale && iterations > 255)
	{
		cout << "Unable to compute a grayscale image with iterations > 255" << endl
			<< "Number of iterations will temporarily be changed to 255." << endl << endl;
	}
	if (height > width)
	{
		cout << "Expected width to be greater than height. Output will not contain expected width." << endl
			<< "I can fix this in mkII for extra credit :)" << endl << endl;
	}
	for (size_t i = 0; i < test_list.size(); i++)
	{
		num_threads = test_list[i];
		if (num_threads > height)
		{
			num_threads = height;
			cout << endl << "Number of threads cannot exceed the height!" << endl
				<< "Reducing the number of threads to match the height (" << height << ")." << endl;

		}
		Stopwatch timer = launch_multibrot_test();
		if (num_threads == 0)
		{
			cout << "The dedicated singlethread function took: " << timer.elapsed() << " seconds." << endl;
		}
		else
		{
			cout << "Time to compute the mandelbrot set with " << num_threads << " thread(s): " << timer.elapsed() << " seconds." << endl;
		}
		
	}
	if (write)
	{
		cout << endl << "Writing results to image file." << endl;
		Stopwatch timer = write_image_file();
		cout << "Time to write header " << timer.lap_times[0] << " seconds." << endl;
		cout << "Time to write results to Disk: " << timer.elapsed() << " seconds." << endl
			<< "Results stored in: " << filename << endl;
	}
}

/*
This function resets the array and launches the number of threads specified in the num_threads global variable
and returns a timer object that contains the time it took to compute the mandelbrot set with the specified
number of threads.
*/
Stopwatch launch_multibrot_test()
{
	// Initialize variables
	Stopwatch timer;
	array = vector<vector<char>>(0, vector<char>(0, 0)); // Reset array to empty vector because of issues with larger vectors
	array = vector<vector<char>>(height, vector<char>(width, 0));
	x_offset = 0;
	if (1 < zoom && zoom <= 2)
	{
		x_offset = (zoom - 1) / 2;
	}
	else if (zoom > 2)
	{
		x_offset = .5;
	}

	y_start_coord = 2 / zoom;
	scale = 2 * y_start_coord / height;
	x_start_coord = -scale*width / 2;

	// Launch singlethread mandelbrot set computation function
	if (num_threads == 0)
	{
		timer.start();
		singlethread_mandelbrot();
		timer.stop();
	}

	// Launch multithreaded mandelbrot set computation function
	else
	// Source 6: Used to figure out the best way to use threads in C++11
	{
		// Make thread handlers and start threads
		vector<thread> handlers(num_threads);

		timer.start();
		for (unsigned i = 0; i < num_threads; i++)
			handlers[i] = thread(multithread_mandelbrot, i);

		for (unsigned i = 0; i < num_threads; i++)
			handlers[i].join();	// Wait for threads to complete before outputting array to file
		timer.stop();
	}
	return timer;
}

/*
This function looks at the num_threads global variable and its own thread id (tid)
and figures out which rows it needs to compute of the mandelbrot set.
*/
void multithread_mandelbrot(unsigned tid)
{
	unsigned min_rows = height / num_threads;
	unsigned extra_rows = height % num_threads;

	// Find out how many rows this thread will compute and what row it will start in
	unsigned num_rows = min_rows;
	unsigned current_row = tid*min_rows;
	if (extra_rows > tid) // The first extra_rows threads will calculate an extra row of data
	{
		num_rows++;
		current_row += tid;
	}
	else
		current_row += extra_rows;
	unsigned last_row = current_row + num_rows;

	// Find out which y_coordinate to start computing at
	double y_coord = y_start_coord - scale / 2; // Start at the coordinate at the middle of the pixel
	y_coord -= current_row*scale;

	double x_coord; // TODO: is it cheaper to declare outside the loop?
	for (current_row; current_row < last_row; current_row++)
	{
		x_coord = x_start_coord - scale / 2 - x_offset; // Start at the coordinate in the middle of the pixel
		for (unsigned current_column = 0; current_column < width; current_column++)
		{
			array[current_row][current_column] = mandelbrot_escape_time(x_coord, y_coord);
			x_coord += scale;
		}
		y_coord -= scale;
	}
}

/*
This function computes the mandelbrot set with 1 thread only
*/
void singlethread_mandelbrot()
{
	double y_coord = y_start_coord - scale / 2;
	double x_coord;
	for (unsigned current_row = 0; current_row < height; current_row++)
	{
		x_coord = x_start_coord - scale / 2 - x_offset; // x_start_coord is a global
		for (unsigned current_column = 0; current_column < width; current_column++)
		{
			array[current_row][current_column] = mandelbrot_escape_time(x_coord, y_coord);
			x_coord += scale;
		}
		y_coord -= scale;
	}
}

/*
Given an x and y coordinate this function returns the escape time if it escapes within the
max number of iterations, else it returns 0.
*/
char mandelbrot_escape_time(double x0, double y0)
// Source 3: Changed from returning an int to returning a char at Professor Ordonez's suggestion.
{
	double x = 0;
	double y = 0;
	double xtemp;
	int brightness = iterations;
	if (grayscale && iterations > 255)
	{
		brightness = 255;
	}

	while (x*x + y*y < 2 * 2 && brightness > 0)
	// Source 4: Used the pseudocode straight from wikipedia for calculating escape time.
	{
		xtemp = x*x - y*y + x0;
		y = 2 * x*y + y0;
		x = xtemp;
		brightness--;
	}
	if (!grayscale && brightness > 0)
	{
		return 255;
	}
	else
	{
		return brightness;
	}
}

/*
Takes the results array and outputs a portable grayscale map (pgm) file. Returns a timer containing the time it took to do so.
*/
Stopwatch write_image_file()
// Source 2 used for whole function.
{
	Stopwatch timer;

	// Source 1 used for writing header of pgm file:
	ofstream myfile;
	timer.start();
	myfile.open(filename);
	myfile << "P5\n" << width << ' ' << height << '\n' << min(iterations, 255) << " ";
	myfile.close();
	timer.lap();

	// Sources 8 & 9: Used to discover and learn how to use fopen and fwrite
	FILE* pfile;
	pfile = fopen(filename, "ab");

	for (size_t i = 0; i < array.size(); i++)
	{
		fwrite(array[i].data(), 1, array[i].size(), pfile);
	}

	fclose(pfile);
	timer.stop();

	return timer;
}

/*
Displays the current test parameters, I made this a function because I display the test parameters in 2 places 
as an option in ui() and automatically when I run a test list.) and I got tired of updating both places 
whenever I added new test parameters.
*/
void display_parameters()
{
	cout << endl << "Test Parameters:" << endl
		<< "Resolution: " << width << "x" << height << " (WxL)." << endl
		<< "Max number of iterations: " << iterations << endl
		<< "Zoom level: " << zoom << endl
		<< "Grayscale or B/W(1/0): " << grayscale << endl
		<< "Write output to file(1/0): " << write << endl
		<< "High priority(1/0): " << hi_priority << endl << endl;
}