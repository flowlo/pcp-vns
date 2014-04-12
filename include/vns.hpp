/// This file contains all definitions for the Variable Neighborhood Search
/// for the PC-Problem
#ifndef VNS_H
#define VNS_H
#include "pcp.hpp"
#include "Solution.hpp"
#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/functional/hash.hpp>

namespace pcp {
	//! Start optimizing an existing solution using Variable Neighborhood Search
	/*!
		Takes an initial solution of the pcp and tries to optimize it by
		applying Variable Neighborhood Search.
		\param best The initial solution
		\param units A string representing the order of neighborhoods
		\param unsuccessfulShake The number of unsuccessful shakes before the vns terminates
		\param shakeStart The initial severity of the shaking process
		\param shakeIncrement The increase in severity of shaking
		\param maxTime The maximum amount of time before the vns terminates, in seconds
		\param checkIntermediate Wheter solutions should be checked for validity on the fly or not
		\param checkFinal Wheter the best solution after vns completion should be checked for validity or not
		\return The best solution the vns could find
	*/
	Solution vns(Solution& best, std::string units, int unsuccessfulShake, int shakeStart, int shakeIncrement, int maxTime, bool checkIntermediate, bool checkFinal);
}

#endif
