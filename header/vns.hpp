/// This file contains all definitions for the Variable Neighborhood Search
/// for the PC-Problem
#ifndef VNS_H
#define VNS_H
#include "pcp.hpp"
#include <vector>
#include <boost/unordered_set.hpp>

namespace pcp {
	/// Run the VNS with a maximum of k iterations.
	/// Returns after either there were a number of unsuccessful shakes 
	/// or if 'time' seconds have elapsed.
	/// Shaking starts with shakeStart steps and is then incremented by
	/// shakeIncrement.
	Solution *vnsRun(Solution& best, Solution& orig, std::string units, int unsuccessfulShake, int shakeStart, int shakeIncrement, int maxTime, bool checkIntermediate, bool checkFinal);
}

#endif
