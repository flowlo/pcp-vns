#include "../header/vns-priv.hpp"

using namespace pcp;
using namespace std;

const char* tabuSearch::name() {
	return "Tabu Search";
}

Solution* tabuSearch::findLocalMin(Solution& curBest, Solution& full) {
	cout<<"Hello this is tabusearch speaking"<<endl;
	return new Solution(&full);			
}

Solution* tabuSearch::shuffleSolution(Solution& cur, Solution& full, int numSteps) {
	return &full;
}
