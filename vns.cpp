/// This file contains the implementation of VNS
#include "header/vns-priv.hpp"

using namespace std;

namespace pcp {
	Solution bestSolution;
	Solution origSolution;

	bool checkValid(Solution* s);

	/// Some constants
	const int NUM_VNS = 2;
	const int SHAKE_START = 1;
	
	/// Implementation of VNS, see vns.hpp
	Solution vnsRun(Solution& best, Solution& orig, char* units, int unsuccessfulShake, 
						 int shakeStart, int shakeIncrement, int maxTime) {

		/// Backup the starting solutions
		bestSolution = new Solution(best);
		origSolution = new Solution(orig);
		
		if (DEBUG_LEVEL > 1) {
			cout<<"Best solution uses "<<bestSolution.colorsUsed<<" colors"<<endl;
			cout<<"Full solution uses "<<origSolution.colorsUsed<<" colors"<<endl;
		}
		if (DEBUG_LEVEL > 1) {
			cout<<"The supplied solution is valid: "<<(checkValid(&best) ? "true": "false")<<endl;
		}
		
		vector<VNS_Unit*> neighbors = vector<VNS_Unit*>();
		
		while (*units != '\0') {
			if (*units == changeNode::abbreviation())
				neighbors.push_back(new changeNode());
			else if (*units == changeColor::abbreviation())
				neighbors.push_back(new changeColor());
			else if (*units == tabuSearch::abbreviation())
				neighbors.push_back(new tabuSearch());
			else {
				cerr << "Invalid unit specified. " << *units << endl;
				return NULL;
			}
			units++;
		}
		units = NULL;

		
		/// Initialize stat-tracking arrays
		int impStats[NUM_VNS];
		int clockStats[NUM_VNS];
		int runStats[NUM_VNS];
	
		time_t startTime = time(NULL);
		int no_imp_runs = 0;
		unsigned int curNeighbor = 0;
		int shakeSteps = shakeStart - shakeIncrement;
		Solution *toImprove = new Solution(&best);
		Solution *curBest = &best;
		
		/// Run as long as shaking still produces usefull solution
		while (true) {
			
			/// Run all possible neighborhood
			while (curNeighbor < neighbors.size()) {
				
				/// Select the new neighborhood
				clock_t start = clock();
				VNS_Unit *neigh = neighbors[curNeighbor];
				
				if (DEBUG_LEVEL > 1) {
					cout<<"Running: "<< neigh->name() <<endl;
				}
				
				/// Compute the minimum for this neighborhood
				Solution *improved = neigh->findLocalMin(*toImprove, orig);
				
				/// Stats tracking
				impStats[curNeighbor] += (toImprove->colorsUsed - improved->colorsUsed);
				clockStats[curNeighbor] += (clock() - start);
				runStats[curNeighbor]++;
				
				if (DEBUG_LEVEL > 1) {
					cout<<neigh->name()<<" took about ";
					cout<<(clock() - start)/(float)CLOCKS_PER_SEC;
					cout<<" seconds to complete"<<endl;
					cout<<"Valid solution: "<<((checkValid(improved)) ? "true" : "false");
					cout<<endl;
				}
				/// Replace the existing solution with the new solution if it is an
				/// improvement
				if (improved->colorsUsed < toImprove->colorsUsed) {
					delete toImprove;
					toImprove = improved;
					
					if (DEBUG_LEVEL > 1) {
						cout<<"Improvement found with "<<neigh->name()<<endl;
						cout<<"Current solution uses "<<toImprove->colorsUsed<<" colors"<<endl;
					}
					
					/// Restart neighborhoods
					curNeighbor = curNeighbor == 0 ? 1 : 0;
				}
				
				/// Step to next neighborhood, no improvement found
				else {
					if (DEBUG_LEVEL > 1) {
						cout<<"No improvement found"<<endl;
						cout<<"Test next neighborhood"<<endl;
					}
					delete improved;
					curNeighbor++;
				}
				if (DEBUG_LEVEL > 1) {
					cout<<endl;
					cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
					cout<<endl<<endl;
				}
			} // end while neighborhood
			
			if (DEBUG_LEVEL > 1) {
				cout<<endl;
				cout<<"###################### End inner VNS loop ##################"<<endl;
				cout<<endl;
			}
			
			/// Local minimum of neighborhoods is better than current best
			/// solution
			if (toImprove->colorsUsed < best.colorsUsed) {
				if (DEBUG_LEVEL > 1) {
					cout<<"Improvement to global best solution found"<<endl;
				}
				
				best = toImprove;
				toImprove = new Solution(curBest);
				no_imp_runs = 0;
				shakeSteps = shakeStart - shakeIncrement;
			}
			/// Reset local best solution to global best Solution
			else {
				toImprove = curBest;
				no_imp_runs++;

				/// Stopping condition, quit VNS
				if (no_imp_runs > unsuccessfulShake) {
					if (DEBUG_LEVEL > 0) {
						cout<<"Too many unsuccessful shakes"<<endl;
					}
					break;
				}
			}
			
			int shakeNeighbor = rand() % NUM_VNS;
			curNeighbor = 0;
			VNS_Unit *shaker = neighbors[shakeNeighbor];
			toImprove = shaker->shuffleSolution(*toImprove, orig, (shakeSteps += shakeIncrement));
			
			if (DEBUG_LEVEL > 1) {
				cout<<"Shaking Solution using "<<shaker->name()<<" with ";
				cout<<shakeSteps<<" steps"<<endl;
				cout<<"Shake was valid: "<<((checkValid(toImprove)) ? "true" : "false")<<endl;
				cout<<"Solution now uses "<<toImprove->colorsUsed<<" colors"<<endl;
			}
	
			/// No time left, abort main loop
			if (startTime + maxTime < time(NULL)) {
				if (DEBUG_LEVEL > 0) {
					cout<<"Time's up"<<endl;
				}
				break;
			}
		}
		if (DEBUG_LEVEL > 0) {
			cout<<endl;
			cout<<"Final best solution uses "<<curBest->colorsUsed<<" colors";
			cout<<endl;
			cout<<"The solution appears to be ";
			cout<<((checkValid(curBest)) ? "valid" : "invalid")<<endl;
		}
		/// Print stats
		if (DEBUG_LEVEL > 1) {
			cout<<endl;
			cout<<"#################### STAT TRACKING ####################"<<endl;
			cout<<"#                                                     "<<endl;
			
			for (int i = 0; i < NUM_VNS; i++) {
				VNS_Unit *cur = neighbors[i];
				cout<<"# "<<cur->abbreviation()<<": Name: "<<cur->name()<<endl;
				cout<<"# Runs: "<<runStats[i]<<" runtime: ";
				cout<<((float)clockStats[i]/CLOCKS_PER_SEC)<<" imrovements: ";
				cout<<impStats[i]<<endl;
				cout<<"#                                                     "<<endl;
			}
			cout<<"# Global improvment of ";
			cout<<(bestSolution.colorsUsed - curBest->colorsUsed);
			cout<<" colors, down to "<<curBest->colorsUsed<<" colors "<<endl;
			cout<<"#                                                     "<<endl;
			cout<<"##################### END TRACKING ####################"<<endl;
		}
		
		return new Solution(curBest);
	}
	
	/// validate solutions
	bool checkValid(Solution* s) {
		pair<VertexIter, VertexIter> vIter;
		int parts[s->numParts];
		int colors[s->numParts];
		typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
		VertexPart_Map vParts = get(boost::vertex_index1_t(), *s->g);
		bool valid = true;
	
		/// Initialize parts and colors
		for (int i = 0; i < s->numParts; i++) {
			parts[i] = 0;
			colors[i] = 0;
		}
	
		/// Check all vertices
		for (vIter = vertices(*s->g); vIter.first != vIter.second; vIter.first++) {
			/// Mark partition and color as used
			parts[vParts[*vIter.first]] = 1;
			colors[s->partition[vParts[*vIter.first]]] = 1;
			
			/// Check color conflicts
			pair<AdjIter, AdjIter> aIter;
			for (aIter = adjacent_vertices(*vIter.first, *s->g); 
				  aIter.first != aIter.second; aIter.first++) {
			
				if (s->partition[vParts[*aIter.first]] == 
					 s->partition[vParts[*vIter.first]]) {
				
					valid = false;
					cerr<<"Solution is invalid"<<endl;
					cerr<<"Adjacent vertices "<<*aIter.first<<" and "<<*vIter.first;
					cerr<<" have same color "<<s->partition[vParts[*vIter.first]];
					cerr<<endl;
				}
			}
		}
		
		/// Check colorsUsed
		int count = 0;
		for (int i = 0; i < s->numParts; i++) {
			if (colors[i] == 1) {
				count++;
			}
		}
		if (count != s->colorsUsed) {
			valid = false;
			cerr<<"Solution is invalid"<<endl;
			cerr<<"Wrong colorsUsed stored: stored: "<<s->colorsUsed;
			cerr<<", computed: "<<count<<endl;
		}
		
		/// Check partitions and representatives
		for (int i = 0; i < s->numParts; i++) {
			if (parts[i] == 0) {
				valid = false;
				cerr<<"Solution is invalid"<<endl;
				cerr<<"partition "<<i<<" seems to be missing"<<endl;
			}
			if (s->representatives[vParts[i]] != i) {
				valid = false;
				cerr<<"Solution is invalid"<<endl;
				cerr<<"Node "<<i<<" is not representative of partition "<<vParts[i];
				cerr<<endl;
			}
		}
	
		return valid;
	}
}

