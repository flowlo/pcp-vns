#ifndef VNS_PRIV_H
#define VNS_PRIV_H
#include "pcp.hpp"
#include "vns.hpp"
#include <time.h>

namespace pcp {
	/// every neighborhood has to be inherited from this class
	class VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name();
			
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			
			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);
	};
	
	class dummy : public VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name();
			
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			
			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);
	};
	
	/// Implement a dummy tabu search, see VNS_Unit
	class tabuSearch : public VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name();
			
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			
			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);
	};
}
#endif
