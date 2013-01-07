/// This file contains the implementation of VNS
#include "header/vns-priv.hpp"

using namespace std;

namespace pcp {
	Solution bestSolution;
	Solution origSolution;

	bool checkValid(Solution* s, Solution* full);
	
	/// Implementation of VNS, see vns.hpp
	Solution *vnsRun(Solution& best, Solution& orig, string units, int unsuccessfulShake, 
						 int shakeStart, int shakeIncrement, int maxTime) {

		/// Backup the starting solutions
		bestSolution = new Solution(best);
		origSolution = new Solution(orig);
		
		if (DEBUG_LEVEL > 1) {
			cout<<"Best solution uses "<<bestSolution.colorsUsed<<" colors"<<endl;
			cout<<"Full solution uses "<<origSolution.colorsUsed<<" colors"<<endl;
		}
		if (DEBUG_LEVEL > 1) {
			cout<<"The supplied solution is valid: "<<(checkValid(&best, &orig) ? "true": "false")<<endl;
		}
		
		vector<VNS_Unit*> neighbors = vector<VNS_Unit*>();
		
		for (string::iterator i = units.begin(); i < units.end(); i++) {
			if (*i == changeNode::abbreviation())
				neighbors.push_back(new changeNode());
			else if (*i == changeColor::abbreviation())
				neighbors.push_back(new changeColor());
			else if (*i == tabuSearch::abbreviation())
				neighbors.push_back(new tabuSearch());
			else if (*i == dsatur::abbreviation())
				neighbors.push_back(new dsatur());
			else {
				cerr << "Invalid unit specified. " << units << endl;
				return NULL;
			}
		}
		
		vector<pair<int, int>> stats[neighbors.size()];
	
		time_t startTime = time(NULL);
		int no_imp_runs = 0, improvement;
		unsigned int curNeighbor = 0;
		int shakeSteps = shakeStart - shakeIncrement;
		Solution *toImprove = new Solution(&best);
		Solution *curBest = &best;
		
		/// Run as long as shaking still produces usefull solution
		while (true) {
			
			/// No time left, abort main loop
			if (startTime + maxTime < time(NULL)) {
				if (DEBUG_LEVEL > 0) {
					cout<<"Time's up"<<endl;
				}
				break;
			}
			
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
				
				improvement = toImprove->colorsUsed < 1 ? 0 : toImprove->colorsUsed - improved->colorsUsed;
				
				/// Stats tracking
				stats[curNeighbor].push_back(pair<int, int>(clock() - start, improvement));
				
				if (DEBUG_LEVEL > 1) {
					cout<<neigh->name()<<" took about ";
					cout<<(clock() - start)/(float)CLOCKS_PER_SEC;
					cout<<" seconds to complete"<<endl;
					cout<<"Valid solution: "<<((checkValid(improved, &orig)) ? "true" : "false");
					cout<<endl;
				}
				/// Replace the existing solution with the new solution if it is an
				/// improvement
				if (improvement > 0) {
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
			
			int shakeNeighbor = rand() % neighbors.size();
			curNeighbor = 0;
			VNS_Unit *shaker = neighbors[shakeNeighbor];
			toImprove = shaker->shuffleSolution(*toImprove, orig, (shakeSteps += shakeIncrement));
			
			if (DEBUG_LEVEL > 1) {
				cout<<"Shaking Solution using "<<shaker->name()<<" with ";
				cout<<shakeSteps<<" steps"<<endl;
				cout<<"Shake was valid: "<<((checkValid(toImprove, &orig)) ? "true" : "false")<<endl;
				cout<<"Solution now uses "<<toImprove->colorsUsed<<" colors"<<endl;
			}
		}
		if (DEBUG_LEVEL > 0) {
			cout<<endl;
			cout<<"Final best solution uses "<<curBest->colorsUsed<<" colors";
			cout<<endl;
			cout<<"The solution appears to be ";
			cout<<((checkValid(curBest, &orig)) ? "valid" : "invalid")<<endl;
		}
		/// Print stats
		if (DEBUG_LEVEL > 1) {
			cout << "{" << endl;
			cout << "  \"units\": \"" << units << "\"," << endl;
			cout << "  \"improvement\" : " << (bestSolution.colorsUsed - curBest->colorsUsed) << "," << endl;
			cout << "  \"colors\" : " << curBest->colorsUsed << "," << endl;
			cout << "  \"stats\": [" << endl;
		
			for (unsigned int i = 0; i < neighbors.size(); i++) {
				int impCount;
				pair<int, int> all, imp;
				for (vector<pair<int, int>>::iterator j = stats[i].begin(); j < stats[i].end(); j++) {
					all.first += (*j).first;
					all.second += (*j).second;
					if ((*j).second > 0) {
						imp.first += (*j).first;
						imp.second += (*j).second;
						impCount++;
					}
				}
				
				pair<double, double> allAvg, impAvg;
				allAvg.first = all.first / (double)stats[i].size();
				allAvg.second = all.second / (double)stats[i].size();
				impAvg.first = imp.first / (double)impCount;
				impAvg.second = imp.second / (double)impCount;
				
				pair<double, double> allDev, impDev;
				for (vector<pair<int, int>>::iterator j = stats[i].begin(); j < stats[i].end(); j++) {
					allDev.first += pow((double)((*j).first - allDev.first), 2);
					allDev.second += pow((double)((*j).second - allDev.second), 2);
					
					if ((*j).second > 0) {
						impDev.first += pow((double)((*j).first - impDev.first), 2);
						impDev.second += pow((double)((*j).second - impDev.second), 2);
					}
				}
				
				allDev.first = sqrt(allDev.first / (double)stats[i].size());
				allDev.second = sqrt(allDev.second / (double)stats[i].size());
				impDev.first = sqrt(impDev.first / (double)impCount);
				impDev.second = sqrt(impDev.second / (double)impCount);
			
				VNS_Unit *cur = neighbors[i];
				
				cout << "    {" << endl;
				cout << "      \"name\" : \"" << cur->name() << "\"," << endl;
				cout << "      \"abbrevition\" : \"" << cur->abbreviation() << "\"," << endl;
				cout << "      \"all\" : {" << endl;
				cout << "        \"runs\" : " << stats[i].size() << "," << endl;
				cout << "        \"time\" : { " << endl;
				cout << "          \"sum\" : " << all.first << "," << endl;
				cout << "          \"avg\" : " << allAvg.first << "," << endl;
				cout << "          \"dev\" : " << allDev.first << endl;
				cout << "        }," << endl;
				cout << "        \"improvements\" : { " << endl;
				cout << "          \"sum\" : " << all.second << "," << endl;
				cout << "          \"avg\" : " << allAvg.second << "," << endl;
				cout << "          \"dev\" : " << allDev.second << endl;
				cout << "        }" << endl;
				cout << "      }," << endl;
				cout << "      \"improving\" : {" << endl;
				cout << "        \"runs\" : " << impCount << "," << endl;
				cout << "        \"time\" : { " << endl;
				cout << "          \"sum\" : " << imp.first << "," << endl;
				cout << "          \"avg\" : " << impAvg.first << "," << endl;
				cout << "          \"dev\" : " << impDev.first << endl;
				cout << "        }," << endl;
				cout << "        \"improvements\" : { " << endl;
				cout << "          \"sum\" : " << imp.second << "," << endl;
				cout << "          \"avg\" : " << impAvg.second << "," << endl;
				cout << "          \"dev\" : " << impDev.second << endl;
				cout << "        }" << endl;
				cout << "      }" << endl;
				cout << "    }";
				
				if (i != neighbors.size() - 1)
					cout << ",";
					
				cout << endl;
			}
			
			cout << "  ]" << endl;
			cout << "}" << endl;
		}
		
		return new Solution(curBest);
	}
	
	/// validate solutions
	bool checkValid(Solution* s, Solution *full) {
		pair<VertexIter, VertexIter> vIter;
		int parts[s->numParts];
		int colors[s->numParts];
		typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
		pair<AdjIter, AdjIter> aIter;
		VertexPart_Map vParts = get(boost::vertex_index1_t(), *s->g);
		bool valid = true;
		list<Vertex> allOrigIds;
	
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
			
			allOrigIds.push_back(s->getOriginalId(*vIter.first));
			
			/// Check color conflicts
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
		
		/// Compares the solution to the full graph, to report missing edges, 
		/// wrong adjacency and other bad stuff
		list<Vertex> origAdj;
		list<Vertex> curAdj;
		
		for (vIter = vertices(*s->g); vIter.first != vIter.second; 
			  vIter.first++) {
			
			origAdj.clear();
			curAdj.clear();
			
			Vertex toCompare = *vIter.first;
			Vertex origComp = s->getOriginalId(toCompare);
			
			/// Check if the partitions match
			if (s->getPartition(toCompare) != full->getPartition(origComp)) {
				cerr<<"Vertex "<<toCompare<<" has other partition than the ";
				cerr<<"original vertex "<<origComp<<endl;
			}
			
			/// Fill in all adjacencies in the original graph
			for (aIter = adjacent_vertices(origComp, *full->g); 
				  aIter.first != aIter.second; aIter.first++) {
				
				origAdj.push_back(*aIter.first);
			}
			
			/// Fill in all current adjacencies
			for (aIter = adjacent_vertices(toCompare, *s->g); 
				  aIter.first != aIter.second; aIter.first++) {
				
				curAdj.push_back(*aIter.first);
			}
			
			/// Check if all current adjacencies are part of the original graph
			list<Vertex>::iterator lIter, found;
			for (lIter = curAdj.begin(); lIter != curAdj.end(); lIter++) {
				found = find(origAdj.begin(), origAdj.end(), 
								 s->getOriginalId(*lIter));
								 
				if (found == origAdj.end()) {
					cerr<<"Edge ("<<*lIter<<"|"<<toCompare<<") is not in the";
					cerr<<" original graph"<<endl;
					valid = false;
				}
				else {
					origAdj.erase(found);
				}
			}
			
			/// Check if there are missing adjacencies
			for (lIter = origAdj.begin(); lIter != origAdj.end(); lIter++) {
			
				found = find(allOrigIds.begin(), allOrigIds.end(), *lIter);
				if (found != allOrigIds.end()) {
					cerr<<"Vertex "<<toCompare<<" seems to be an missing edge to ";
					cerr<<"original vertex "<<*found<<endl;
					valid = false;
				}
			}
		}
		
	
		return valid;
	}
}

