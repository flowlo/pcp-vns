/// This file contains all neccessary definitions to perform the onestep color-
/// degree contruction heuristic

#ifndef ONE_STEP_H
#define ONE_STEP_H
#include "pcp.hpp"

namespace pcp {
	/// Takes an existing full graph and performs the onestep color-degree 
	/// heuristic on a copy of the existing graph, and returns the newly found
	/// solution
	Solution onestepCD(Solution& s);
	
	/// Remove all vertices from a partition, with the exception of the given
	/// vertex node.
	void removeOthers(Vertex node, Solution& s);
	
	/// Remove all edges between vertices of the same partition.
	void removePartEdges(Solution& s);
	
	/// Returns the minimal possible color for the given vertex node, without 
	/// starting a color conflict.
	int minPossibleColor(Vertex node, Solution& s);
	
	/// Return the number of colored adjacencies of the given vertex node
	int colorDegree(Vertex node, Solution& s);
}

#endif
