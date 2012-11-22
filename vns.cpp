#include "header/vns.hpp"
#define NUM_VNS 1

using namespace std;

namespace pcp {
	Solution bestSolution;
	Solution origSolution;

	class VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest, Solution& full) {
				return curBest;
			}
	};
	
	class tabuSearch : public VNS_Unit {
		public:
			virtual Solution findLocalMin(Solution& curBest, Solution& full) {
				cout<<"Hello this is tabusearch speaking"<<endl;
				return full;			
			}
	};

	/// Run the VNS with a maximum of k iterations.
	/// Returns after either k iterations of the vns loop 
	/// or if 'time' milliseconds have elapsed.
	Solution vnsRun(Solution& best, Solution& orig, int k, int time) {
		bestSolution = best;
		origSolution = orig;
		
		cout<<"Best solution uses "<<bestSolution.colorsUsed<<" colors"<<endl;
		cout<<"Full solution uses "<<origSolution.colorsUsed<<" colors"<<endl;
		
		VNS_Unit **neighbors = new VNS_Unit*[NUM_VNS];
		tabuSearch *ts = new tabuSearch;
		neighbors[0] = ts;
		
		int runs = 0;
		int curNeighbor = 0;
		
		while (runs++ < k) {
			Solution tempBest = best;
			// TODO: randomize solution
			
			cout<<"Run "<<runs-1<<" VNS"<<endl;
			while (curNeighbor < NUM_VNS) {
				cout<<"Use "<<curNeighbor<<" neighborhood"<<endl;
				Solution temp = neighbors[curNeighbor++]->findLocalMin(tempBest, orig);
				cout<<"It uses "<<temp.colorsUsed<<" colors"<<endl;
				if (temp.colorsUsed <= tempBest.colorsUsed) {
					cout<<"Found new best solution"<<endl;
					tempBest = temp;
					if (curNeighbor != 0)
						curNeighbor = 0;
				}
			}
			if (tempBest.colorsUsed < best.colorsUsed) {
				best = tempBest;
			}
		}
	
		return best;
	}
}

