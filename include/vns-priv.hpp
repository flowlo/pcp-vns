/// Contains all definitions of units for different neighborhoods of the VNS
#ifndef VNS_PRIV_H
#define VNS_PRIV_H
#include "pcp.hpp"
#include "vns.hpp"
#include <time.h>
#include <utility>
#include <boost/tuple/tuple.hpp>
#include <signal.h>

namespace pcp {
	/// every neighborhood has to be inherited from this class
	class VNS_Unit {
		public:
			/// Compute the new improved solution of this neighborhood
			virtual Solution findLocalMin(Solution& curBest) = 0;

			/// Shuffle a solution using the neighborhood as a base
			virtual Solution shuffleSolution(Solution& cur, int numSteps) = 0;

			virtual ~VNS_Unit();

			/// Returns a given name for the neighborhood
			virtual const std::string getName() = 0;

			/// Returns a given (unique) character used to quickly reference
			/// an unit via command line argument.
			static const char getAbbreviation();
	};

	// Definition of changeColor neighborhood
	class changeColor : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};

	// Definition of changeNode neighborhood
	class changeNode : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};

	// Definition of changeAll neighborhood
	class changeAll : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};

	// Definition of DSATUR neighborhood
	class dsatur : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest);
			virtual Solution shuffleSolution(Solution& cur, int numSteps);

			virtual const std::string getName();
			static const char getAbbreviation();
	};
}
#endif
