#include "../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string changeColor::getName() {
	return "changeColor";
}

const char changeColor::getAbbreviation() {
	return 'c';
}

Solution changeColor::findLocalMin(Solution& best) {
	return best;
}

Solution changeColor::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
