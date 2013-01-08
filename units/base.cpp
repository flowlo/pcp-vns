#include "../header/vns-priv.hpp"

using namespace pcp;
using namespace std;

const string VNS_Unit::getName() {
	return "";
}

const char VNS_Unit::getAbbreviation() {
	return getStaticAbbreviation();
}

const char VNS_Unit::getStaticAbbreviation() {
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
