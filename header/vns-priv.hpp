#ifndef VNS_PRIV_H
#define VNS_PRIV_H
#include "pcp.hpp"
#include "vns.hpp"
#include <time.h>
#include <utility>
#include <boost/tuple/tuple.hpp>

namespace pcp {
	/// every neighborhood has to be inherited from this class
	class VNS_Unit {
		public:
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);

			/// Shuffle a solution using the neighborhood as a base
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);
		
			virtual ~VNS_Unit();
			
			/// Returns a given name for the neighborhood
			virtual const std::string getName();

			/// Returns a given (unique) character used to quickly reference
			/// an unit via command line argument.
			virtual const char getAbbreviation();
			
			static const char getStaticAbbreviation();
	};
	
	class changeColor : public VNS_Unit {
		public:
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);

			virtual const std::string getName();
			virtual const char getAbbreviation();
			static const char getStaticAbbreviation();
	};
	
	class changeNode : public VNS_Unit {
		public:
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);								  
			
			virtual const std::string getName();
			virtual const char getAbbreviation();
			static const char getStaticAbbreviation();
	};
	
	/// Implement a dummy tabu search, see VNS_Unit
	/*class tabuSearch : public VNS_Unit {
		public:
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);
			
			virtual const std::string getName();
			virtual const char getAbbreviation();
			static const char getStaticAbbreviation();
			
		private:
			int conflicts(Solution& s);
	};*/
	
	class dsatur : public VNS_Unit {
		public:
			virtual Solution *findLocalMin(Solution& curBest, Solution& full);
			virtual Solution *shuffleSolution(Solution& cur, Solution& full, int numSteps);

			virtual const std::string getName();
			virtual const char getAbbreviation();
			static const char getStaticAbbreviation();
			
		private:
			int conflicts(Solution& s);
	};
}
#endif
