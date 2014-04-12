#ifndef ONE_STEP_H
#define ONE_STEP_H
// #include "pcp.hpp"
#include "Solution.hpp"

namespace pcp {
	//! Calculate a simple solution for the pcp
	/*!
		Use the one step color degree algorithm to compute an initial solution
		to the pcp which can then be optimized by the vns
		\param s An uncolored solution
		\return A valid solution for the pcp
		\sa vns()
	*/
	Solution onestepCD(Solution& s);
	
	//! Remove other vertices of the same partition
	/*!
		For a given vertices, hide all other vertices of the same partition
		in the filtered graph
		\param x The vertex which shall represent a partition
		\param s The solution on which this operation should be carried out
		\sa onestepCD(Solution& s)
	*/
	void removeOthers(FVertex x, Solution& s);
}

#endif
