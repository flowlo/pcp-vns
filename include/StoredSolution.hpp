// This file contains all necessary definitions for the compressed format
// of a StoredSolution
#ifndef SOL_STORE_H
#define SOL_STORE_H
#include "pcp.hpp"
#include "Solution.hpp"
#include <sstream>
#include <boost/functional/hash.hpp>

namespace pcp {
	// Stores a solution in a memory saving fashion
	struct StoredSolution {
		//public:
			// Construct a StoredSolution out of a normal solution
			StoredSolution(Solution& toStore);
			
			// Destruct a StoredSolution
			~StoredSolution();
			
			// array, equal to partition in solution
			int *colors;
			
			// Contains the representatives of each partition, identified by their
			// original index
			int *representatives;
			
			std::string toString();
			
			//bool operator==(StoredSolution const& s);
			//std::size_t hash_value(StoredSolution const& s);
		//private:
			int n;
	};
	
	// Define a hash function to be used in the hash map
	struct StoredHash
	{
		std::size_t operator()(StoredSolution const& sol) const;
	};
   
   // Define a equality function to compare to StoredSolutions with equal hash
	struct StoredEqual
	{
		bool operator()(StoredSolution const& s1, StoredSolution const& s2) const;
	};
}

#endif
