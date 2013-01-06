#ifndef VNS_PRIV_H
#define VNS_PRIV_H
#include "pcp.hpp"
#include "vns.hpp"
#include <time.h>
#include <utility>   

namespace pcp {
	/// every neighborhood has to be inherited from this class
	class VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name();
			
			/// Returns a given (unique) character used to quickly reference
			/// an unit via command line argument.
			static const char abbreviation();
			
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			
			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);
	};
	
	class changeColor : public VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name();
			
			static const char abbreviation();
			
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			
			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full,
											  int numSteps);
	};
	
	class changeNode : public VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name();
			
			static const char abbreviation();
			
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			
			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full,
											  int numSteps);
	};
	
	/// Implement a dummy tabu search, see VNS_Unit
	class tabuSearch : public VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name();
			
			static const char abbreviation();
			
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			
			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full,
				 							  int numSteps);
		private:
			int conflicts(Solution& s);
	};
}
#endif