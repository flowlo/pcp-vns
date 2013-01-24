#ifndef SOL_STORE_H
#define SOL_STORE_H
#include "pcp.hpp"
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
	
	struct StoredHash
	{
		std::size_t operator()(StoredSolution const& sol) const;
	};
    
	struct StoredEqual
	{
		bool operator()(StoredSolution const& s1, StoredSolution const& s2) const;
	};
}

#endif
