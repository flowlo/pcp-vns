#include "../header/vns-priv.hpp"

using namespace pcp;

/// Returns a given name for the neighborhood
const char* VNS_Unit::name() {
	return "DUMMY";
}

/// Returns a given (unique) character used to quickly reference
/// an unit via command line argument.
const char VNS_Unit::abbreviation() {
	return '\0';
}

/// Compute the new improved solution of this neighborhood
Solution* VNS_Unit::findLocalMin(Solution& curBest, Solution& full) {
	return &curBest;
}

/// Shuffle a solution using the neighborhood as a base
Solution* VNS_Unit::shuffleSolution(Solution& cur, Solution& full, int numSteps) {
	return &full;
}
