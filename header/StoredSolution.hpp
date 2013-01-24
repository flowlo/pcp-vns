#ifndef SOL_STORE_H
#define SOL_STORE_H
#include "pcp.hpp"


namespace pcp {
	// Stores a solution in a memory saving fashion
	class StoredSolution {
		public:
			// Construct a StoredSolution out of a normal solution
			StoredSolution(Solution& toStore);
			
			// Destruct a StoredSolution
			~StoredSolution();
			
			// array, equal to partition in solution
			int *colors;
			
			// Contains the representatives of each partition, identified by their
			// original index
			int *representatives;
		private:
			int n;
	};
}

#endif
