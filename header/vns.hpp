/// This file contains all definitions for the Variable Neighborhood Search
/// for the PC-Problem
#ifndef VNS_H
#define VNS_H
#include "pcp.hpp"
#include <vector>

namespace pcp {
	/// Run the VNS with a maximum of k iterations.
	/// Returns after either there were a number of unsuccessful shakes 
	/// or if 'time' seconds have elapsed.
	/// Shaking starts with shakeStart steps and is then incremented by
	/// shakeIncrement.
	Solution vnsRun(Solution& best, Solution& orig, char* units, int unsuccessfulShake, int shakeStart, int shakeIncrement, int maxTime);
}

#endif