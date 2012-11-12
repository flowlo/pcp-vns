/// This file contains the definition for parsing a .pcp file into a useable
/// format for the pcp problem

#ifndef PCP_PARSER_H
#define PCP_PARSER_H
#include <iostream>
#include "pcp.hpp"
#include <string>
#include <boost/tokenizer.hpp>

namespace pcp {
	/// Reads the input from in stream and stores them in s. On success true is
	/// returned, in case of an error false.
	bool readGraph(std::istream& in, Solution& s);
}
#endif
