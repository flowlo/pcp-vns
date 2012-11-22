#include "header/vns.hpp"
#include <time.h>

using namespace std;

namespace pcp {
	Solution bestSolution;
	Solution origSolution;

	const int NUM_VNS = 1;
	const int SHAKE_START = 1;
	
	class VNS_Unit {
		public:
			virtual const char *name() {
				return "DUMMY";
			}
			virtual Solution *findLocalMin(Solution& curBest, Solution& full) {
				return &curBest;
			}
	};
	
	class tabuSearch : public VNS_Unit {
		public:
			virtual const char *name() {
				return "Tabu Search";
			}
			virtual Solution *findLocalMin(Solution& curBest, Solution& full) {
				cout<<"Hello this is tabusearch speaking"<<endl;
				return new Solution(&full);			
			}
	};

	/// Run the VNS with a maximum of k iterations.
	/// Returns after either there were a number of unsuccessful shakes 
	/// or if 'time' seconds have elapsed.
	Solution vnsRun(Solution& best, Solution& orig, int unsuccessfulShake, int maxTime) {
		bestSolution = new Solution(best);
		origSolution = new Solution(orig);
		
		cout<<"Best solution uses "<<bestSolution.colorsUsed<<" colors"<<endl;
		cout<<"Full solution uses "<<origSolution.colorsUsed<<" colors"<<endl;
		
		VNS_Unit **neighbors = new VNS_Unit*[NUM_VNS];
		tabuSearch *ts = new tabuSearch;
		neighbors[0] = ts;
		
		time_t startTime = time(NULL);
		int no_imp_runs = 0;
		int curNeighbor = 0;
		int shake_val = SHAKE_START;
		bool abort = false;
		Solution *toImprove = new Solution(&best);
		Solution *curBest = &best;
		
		while (!abort) {
			while (curNeighbor < NUM_VNS) {
				VNS_Unit *neigh = neighbors[curNeighbor];
				cout<<"Running: "<< neigh->name() <<endl;
				
				Solution *improved = neigh->findLocalMin(best, orig);
				
				if (improved->colorsUsed < toImprove->colorsUsed) {
					delete toImprove;
					toImprove = improved;
					
					cout<<"Improvement found with"<<neigh->name()<<endl;
					
					if (curNeighbor == 0)
						curNeighbor = 1;
					else
						curNeighbor = 0;
				}
				else {
					
					cout<<"No improvement found"<<endl;
					delete improved;
					curNeighbor++;
					cout<<"Test next neighborhood"<<endl;
				}
			} // end while neighborhood
			
			if (toImprove->colorsUsed < best.colorsUsed) {
				cout<<"Improvement to global best solution found"<<endl;
				curBest = toImprove;
				toImprove = new Solution(curBest);
				no_imp_runs = 0;
				shake_val = SHAKE_START;
			}
			else {
				toImprove = curBest;
			}
			
			if (startTime + maxTime < time(NULL)) {
				cout<<"Time's up"<<endl;
				abort = true;
			}
		}
		return new Solution(curBest);
	}
}

