// This file includes all definitions and includes for the implementation
// of the Solution data type
#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <stack>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include "pcp.hpp"

// additional include when compiled for ubigraph visualization
#ifdef ubigraph
extern "C"{
	#include <UbigraphAPI.h>
}
#endif

#define PARSE_BUFFERSIZE 128
#endif
