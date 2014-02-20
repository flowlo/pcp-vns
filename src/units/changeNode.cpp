#include "../../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string changeNode::getName() {
	return "changeNode";
}

const char changeNode::getAbbreviation() {
	return 'n';
}

Solution changeNode::findLocalMin(Solution& best) {
	return best;
}

Solution changeNode::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
