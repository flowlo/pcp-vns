/// This file contains the implementation of VNS
#include "header/vns.hpp"
#include <time.h>

using namespace std;

namespace pcp {
	Solution bestSolution;
	Solution origSolution;

	/// Some constants
	const int NUM_VNS = 1;
	const int SHAKE_START = 1;
	
	/// every neighborhood has to be inherited from this class
	class VNS_Unit {
		public:
			/// Returns a given name for the neighborhood
			virtual const char *name() {
				return "DUMMY";
			}
			/// Compute the new improved solution of this neighborhood
			virtual Solution *findLocalMin(Solution& curBest, Solution& full) {
				return &curBest;
			}
	};
	
	/// Implement a dummy tabu search, see VNS_Unit
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

	/// Implementation of VNS, see vns.hpp
	Solution vnsRun(Solution& best, Solution& orig, int unsuccessfulShake,
																	int maxTime) {
		/// Backup the starting solutions
		bestSolution = new Solution(best);
		origSolution = new Solution(orig);
		
		cout<<"Best solution uses "<<bestSolution.colorsUsed<<" colors"<<endl;
		cout<<"Full solution uses "<<origSolution.colorsUsed<<" colors"<<endl;
		
		/// Define the neighborhoods to use
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
		
		/// Run as long as shaking still produces usefull solution
		while (!abort) {
			
			/// Run all possible neighborhood
			while (curNeighbor < NUM_VNS) {
				
				/// Select the new neighborhood
				VNS_Unit *neigh = neighbors[curNeighbor];
				cout<<"Running: "<< neigh->name() <<endl;
				
				/// Compute the minimum for this neighborhood
				Solution *improved = neigh->findLocalMin(best, orig);
				
				/// Replace the existing solution with the new solution if it is an
				/// improvement
				if (improved->colorsUsed < toImprove->colorsUsed) {
					delete toImprove;
					toImprove = improved;
					
					cout<<"Improvement found with"<<neigh->name()<<endl;
					
					/// Restart neighborhoods
					if (curNeighbor == 0)
						curNeighbor = 1;
					else
						curNeighbor = 0;
				}
				
				/// Step to next neighborhood, no improvement found
				else {
					cout<<"No improvement found"<<endl;
					delete improved;
					curNeighbor++;
					cout<<"Test next neighborhood"<<endl;
				}
			} // end while neighborhood
			
			/// Local minimum of neighborhoods is better than current best
			/// solution
			if (toImprove->colorsUsed < best.colorsUsed) {
				cout<<"Improvement to global best solution found"<<endl;
				curBest = toImprove;
				toImprove = new Solution(curBest);
				no_imp_runs = 0;
				shake_val = SHAKE_START;
			}
			/// Reset local best solution to global best Solution
			else {
				toImprove = curBest;
			}
			
			/// TODO: Shake that!
			
			/// No time left, abort main loop
			if (startTime + maxTime < time(NULL)) {
				cout<<"Time's up"<<endl;
				abort = true;
			}
		}
		
		return new Solution(curBest);
	}
}

