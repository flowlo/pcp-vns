#include "../header/vns-priv.hpp"

using namespace pcp;
using namespace std;

const char* dummy::name() {
	return "Tabu Search";
}

Solution* dummy::findLocalMin(Solution& curBest, Solution& full) {
	cout<<"Hello this is dummy speaking"<<endl;
	return new Solution(&full);			
}

Solution* dummy::shuffleSolution(Solution& cur, Solution& full, int numSteps) {
	return &full;
}
