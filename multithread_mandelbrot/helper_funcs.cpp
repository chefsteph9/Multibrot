/*
	Stephen Jenks
	helper_funcs.cpp
	2015-11-13: transferred helper functions from multibrot.cpp to this file

*/

#include <stdio.h>
#include "helper_funcs.h"

ostream& operator<<(ostream& os, const vector<unsigned> s)
{
	os << '{';
	if (s.size() > 0)
	{   //  Display the first element
		if (!s.empty())
		{
			os << s[0];
		}
		for (std::size_t i = 1; i < s.size(); i++)
		{
			os << ", " << s[i];
		}
	}
	os << '}';
	return os;
}