#ifndef _IO_HPP_
#define _IO_HPP_

#include "Solution.hpp"
#include <istream>
#include <cstdint>
#include <sstream>

using namespace std;

namespace pcp {
	//! Read a solution from istream
	/*!
		Read a solution from istream. The type of input may be pcp, col or colb.
		\param in The input stream from which the solution should be read.
		\return An uncolored Solution ready to be passed to onestepCD
		\sa readPcPStream(istream& in), readColStream(istream& in), readColBStream(istream& in)
	*/
	Solution readSolution(istream& in);
	//! Read a solution from input stream in pcp format
	Solution readPcpStream(istream& in);
	//! Read solution from input stream in col format
	Solution readColStream(istream& in);
	//! Read solution from input stream in colb format
	Solution readColBStream(istream& in);
	//! Returns a string representing a given solution in human readable form
	string getInfoString(Solution& s);
}

#endif
