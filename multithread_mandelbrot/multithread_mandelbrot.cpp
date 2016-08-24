///*
//Stephen Jenks
//multithread_mandelbrot.cpp : A file to learn how to compute the mandelbrot set using multiple threads
//sources:
//	http://www.cplusplus.com/doc/tutorial/files/ for writing to files
//
//	http://netpbm.sourceforge.net/doc/pgm.html and
//	https://en.wikipedia.org/wiki/Netpbm_format for learning about pgm files
//
//	Professor Ordonez for fixing my skewing pgm files
//
//	https://en.wikipedia.org/wiki/Mandelbrot_set for the pseudocode to calculate the mandelbrot set
//
//	http://www.cplusplus.com/forum/general/103477/ for using int to char to write pure binary to a file
//
//	http://marknelson.us/2012/05/23/c11-threading-made-easy/ 
//	https://solarianprogrammer.com/2011/12/16/cpp-11-thread-tutorial/ For easy ways to do multithreading in C++11
//
//
//2015-11-10 begin
//
//*/
//
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <stdio.h>
//#include <algorithm>
//#include <numeric>
//#include <future>
//#include <thread>
//
//using std::ofstream;
//using std::vector;
//using std::cout;
//using std::cin;
//
//using std::future;
//using std::async;
//using std::launch;
//using std::thread;
//
//
//
//int mandelbrot(double x0, double y0);
//ofstream mandelbrot_from_thread(unsigned tid);
//ofstream& operator<<(ofstream& of, const vector<vector<int>>& s);
//ofstream& operator<<(ofstream& of, vector<future<ofstream>>& s);
//
//vector<vector<int>> array;
//double scale, max_y_val, x_start_coord;
//int width, height, iterations;
//unsigned num_threads;
//
//int _main()
//{
//	cout << "How many threads? ";
//	cin >> num_threads;
//
//	// Initialize variables
//	// 4k = 4096*2304
//	iterations = 35;
//	width =	1920;
//	height = 1080;
//	array = vector<vector<int>>(height, vector<int>(width, 0));
//	max_y_val = 2;
//	double aspect = width / height;
//
//	scale = 2*max_y_val / height;
//	x_start_coord = -scale*width/2;
//
//	vector<future<ofstream>> results(num_threads); //*********************
//	//vector<thread> handlers(num_threads);****************8
//
//	for (unsigned i = 0; i < num_threads; i++)
//	{
//		results[i] = async(launch::async, mandelbrot_from_thread, i); //****************
//		//handlers[i] = thread(mandelbrot_from_thread, i);****************
//	}
//	/*for (unsigned i = 0; i < num_threads; i++)
//	{
//		handlers[i].join();
//	}*/
//	ofstream myfile;
//	myfile.open("test.pgm");
//	myfile << "P5\n" << width << ' ' << height << '\n' << iterations*4 << "\n";
//	myfile << results;  // overloaded function to do this operation is not complete yet
//	myfile << array;
//	myfile.close();
//
//	return 0;
//}
//
//ofstream mandelbrot_from_thread(unsigned tid)
//{
//	double x_coord; // TODO: is it cheaper to declare outside the loop?
//	
//	int min_rows = height / num_threads;
//	unsigned extra_rows = height % num_threads;
//	
//	// Find out how many rows this thread will compute and what row it will start in
//	int num_rows = min_rows;
//	int current_row = tid*min_rows;
//	if (extra_rows > tid) // The first extra_rows threads will calculate an extra row of data
//	{
//		num_rows++;
//		current_row += tid;
//	}
//	else
//		current_row += extra_rows;
//	int last_row = current_row + num_rows;
//
//	// Find out which y_coordinate to start computing at
//	double y_coord = max_y_val;
//	y_coord -= current_row*scale;
//
//	ofstream raw_values;
//	for (current_row; current_row < last_row; current_row++)
//	{
//		x_coord = x_start_coord; // x_start_coord is a global
//		for (int current_column = 0; current_column < width; current_column++)
//		{
//			array[current_row][current_column] = mandelbrot(x_coord, y_coord);
//			x_coord += scale;
//		}
//		y_coord -= scale;
//	}
//	return raw_values;
//}
//
//
//
//
//int mandelbrot(double x0, double y0)
//{
//	double x = 0;
//	double y = 0;
//	double xtemp;
//	int brightness = iterations;
//
//	while (x*x + y*y < 2 * 2 && brightness > 0)
//	{
//		xtemp = x*x - y*y + x0;
//		y = 2 * x*y + y0;
//		x = xtemp;
//		brightness--;
//	}
//	return brightness * 4;
//}
//
//ofstream& operator<<(ofstream& of, const vector<vector<int>>& s)
//{
//	int rows = s.size();
//	int columns = s[0].size();
//
//	for (int y = 0; y < rows; y++)
//	{
//		for (int x = 0; x < columns; x++)
//		{
//			of << char(s[y][x]);
//		}
//	}
//
//	return of;
//}
//
//// STUB
//ofstream& operator<<(ofstream& of, vector<future<ofstream>>& s)
//{
//	for (unsigned i = 0; i < s.size(); i++)
//	{
//		auto test = s[i].get();
//	}
//	return of;
//}