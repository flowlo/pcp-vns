#ifndef _IO_HPP_
#define _IO_HPP_

#include "Solution.hpp"
#include <istream>
#include <cstdint>
#include <sstream>

using namespace std;

namespace pcp {
	Solution readSolution(istream& in);
	Solution readPcpStream(istream& in);
	Solution readColStream(istream& in);
	Solution readColBStream(istream& in);
	string getInfoString(Solution& s);
}

#endif
