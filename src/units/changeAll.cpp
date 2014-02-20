#include "../../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string changeAll::getName() {
	return "changeAll";
}

const char changeAll::getAbbreviation() {
	return 'a';
}

Solution changeAll::findLocalMin(Solution& best) {
	return best;
}

Solution changeAll::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
