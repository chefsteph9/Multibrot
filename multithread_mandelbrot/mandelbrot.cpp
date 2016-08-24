///*
//Stephen Jenks
//mandelbrot.cpp
//sources:
//http://www.cplusplus.com/doc/tutorial/files/ for writing to files
//
//http://netpbm.sourceforge.net/doc/pgm.html and
//https://en.wikipedia.org/wiki/Netpbm_format for learning about pgm files
//
//Professor Ordonez for fixing my skewing pgm files
//
//https://en.wikipedia.org/wiki/Mandelbrot_set for the pseudocode to calculate the mandelbrot set
//
//http://www.cplusplus.com/forum/general/103477/ for using int to char to write pure binary to a file
//
//
//
//*/
//
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <stdio.h>
//
//using std::ofstream;
//using std::vector;
//using std::cout;
//using std::cin;
//
//
//int mandelbrot(double x0, double y0);
//ofstream& operator<<(ofstream& of, const vector<vector<int>>& s);
//
//int _main()
//{
//	double height = 1080; //9216;
//	double width = 1920; // 16384;
//
//	vector<vector<int>> array(height, vector<int>(width, 0));
//
//	double x_scale = 64 / 9 / width;
//	double y_scale = 4.0 / height;
//
//	double y_coord = 2.0;
//	double x_coord;
//
//	for (int y = 0; y < height; y++)
//	{
//		x_coord = -64 / 18; // TODO: Fix this voodoo number and generalize x_scale and y_scale for multiple aspect ratios
//							// This number is just the starting x value if y goes from 2 to -2 and aspect ratio is 16:9
//		for (int x = 0; x < width; x++)
//		{
//			array[y][x] = mandelbrot(x_coord, y_coord);
//			x_coord += x_scale;
//		}
//		y_coord -= y_scale;
//	}
//	ofstream myfile;
//	myfile.open("test.pgm");
//	myfile << "P5\n" << width << ' ' << height << '\n' << "80\n";
//	myfile << array;
//	myfile.close();
//
//	return 0;
//}
//
//int mandelbrot(double x0, double y0)
//{
//	double x = 0;
//	double y = 0;
//	double xtemp;
//	int brightness = 20;
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