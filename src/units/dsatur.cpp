#include "../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string dsatur::getName() {
	return "dsatur";
}

const char dsatur::getAbbreviation() {
	return 'd';
}

Solution dsatur::findLocalMin(Solution& best) {
	return best;
}

Solution dsatur::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
