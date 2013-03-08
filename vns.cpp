/// This file contains the implementation of VNS
#include "include/vns-priv.hpp"

using namespace std;
using namespace boost;

namespace pcp {
	Solution *bestSolution;
	
	bool checkValid(Solution* s, Solution* full);
	
	bool TERM_SIGINT = false;
	
	void sigint_catch(int signum) {
		TERM_SIGINT = signum == SIGINT;
	}
	
	/// Implementation of VNS, see vns.hpp
	Solution *vnsRun(Solution& best, Solution& orig, string units, int unsuccessfulShake, 
						 int shakeStart, int shakeIncrement, int maxTime, bool checkIntermediate, bool checkFinal) {

		/// Backup the starting solutions
		bestSolution = new Solution(&best);
		
		signal(SIGINT, sigint_catch);
		
		if (DEBUG_LEVEL > 1) {
			cout<<"Best solution uses "<<bestSolution->colorsUsed<<" colors"<<endl;
			cout<<"Full solution uses "<<orig.colorsUsed<<" colors"<<endl;
		}
		if (DEBUG_LEVEL > 1) {
			cout<<"The supplied solution is valid: "<<(checkValid(&best, &orig) ? "true": "false")<<endl;
		}
		
		vector<VNS_Unit*> neighbors = vector<VNS_Unit*>();
		
		for (string::iterator i = units.begin(); i < units.end(); i++) {
			if (*i == changeNode::getStaticAbbreviation())
				neighbors.push_back(new changeNode());
			else if (*i == changeColor::getStaticAbbreviation())
				neighbors.push_back(new changeColor());
			/*else if (*i == tabuSearch::getStaticAbbreviation())
				neighbors.push_back(new tabuSearch());*/
			else if (*i == dsatur::getStaticAbbreviation())
				neighbors.push_back(new dsatur());
			else if (*i == changeAll::getStaticAbbreviation())
				neighbors.push_back(new changeAll());
			else {
				cerr << "Invalid unit specified. " << units << endl;
				return NULL;
			}
		}
		
		vector<pair<int, int> > *stats = new vector<pair<int, int> >[neighbors.size()];
	
		time_t startTime = time(NULL);
		int no_imp_runs = 0, improvement;
		unsigned int curNeighbor = 0;
		int shakeSteps = shakeStart - shakeIncrement;
		Solution *toImprove = &best;
		Solution *curBest = &best;
		
		unordered_set<StoredSolution, StoredHash, StoredEqual> solutionStore;
		
		/// Run as long as shaking still produces usefull solution
		while (!TERM_SIGINT) {
			
			/// No time left, abort main loop
			if (startTime + maxTime < time(NULL)) {
				if (DEBUG_LEVEL > 0) {
					cout << "Time's up! Aborting VNS ..." << endl;
				}
				break;
			}
			
			Solution *tempImp = new Solution(toImprove);
			
			/// Run all possible neighborhood
			while (curNeighbor < neighbors.size() && !TERM_SIGINT) {
				
				/// Select the new neighborhood
				clock_t start = clock();
				VNS_Unit *neigh = neighbors[curNeighbor];
				
				if (DEBUG_LEVEL > 1) {
					cout << "Running " << neigh->getName() << " ..." << endl;
				}
				
				/// Compute the minimum for this neighborhood
				Solution *imp = neigh->findLocalMin(*tempImp, orig);	
				
				improvement = (toImprove->colorsUsed - imp->colorsUsed < 1) ? 0 : toImprove->colorsUsed - imp->colorsUsed;
				
				/// Stats tracking
				stats[curNeighbor].push_back(pair<int, int>(clock() - start, improvement));
				
				if (DEBUG_LEVEL > 1) {
					cout<<neigh->getName()<<" took about ";
					cout<<(clock() - start)/(float)CLOCKS_PER_SEC;
					cout<<" seconds to complete."<<endl;
				}
				/// Replace the existing solution with the new solution if it is an
				/// improvement
				if (improvement > 0) {
					if (toImprove != curBest)
						delete toImprove;

					toImprove = tempImp = imp;
					tempImp = new Solution(tempImp);
					
					StoredSolution *check = new StoredSolution(*imp);
					if (solutionStore.find(*check) != solutionStore.end()) {
						if (DEBUG_LEVEL >= 3) {
							cout << "Detected duplicate solution";
							
							if (DEBUG_LEVEL >= 4)
								cout << " " << check->toString();

							cout << ", proceeding to shake!" << endl;
						}
						curNeighbor = 0;
						delete check;
						break;
					}
					else {
						solutionStore.insert(*check);
						if (DEBUG_LEVEL >= 3) {
							cout << "Added new solution ";
							
							if (DEBUG_LEVEL >= 4)
								cout << check->toString() << " ";

							cout << "to set." << endl;
						}
					}
					
					if (DEBUG_LEVEL > 1) {
						cout << "Improvement found! ";
						cout << "New solution uses " << tempImp->colorsUsed << " colors";

						if (checkIntermediate)
							cout << " and is "<<((checkValid(tempImp, &orig)) ? "valid" : "invalid");

						cout << "." << endl;
					}

					/// Restart neighborhoods
					curNeighbor = curNeighbor == 0 ? 1 : 0;
				}

				/// Step to next neighborhood, no improvement found
				else {
					if (DEBUG_LEVEL > 1)
						cout << "No improvement found." << endl;

					if (toImprove->colorsUsed - imp->colorsUsed < 0) {
						delete tempImp;
						tempImp = new Solution(toImprove);
					}

					curNeighbor++;
				}
				if (DEBUG_LEVEL > 1) {
					cout<<endl;
					cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
					cout<<endl;
				}
			} // end while neighborhood

			if (DEBUG_LEVEL > 1) {
				cout<<endl;
				cout<<"###################### End inner VNS loop ##################"<<endl;
				cout<<endl;
			}
			
			/// Local minimum of neighborhoods is better than current best
			/// solution
			if (toImprove->colorsUsed < curBest->colorsUsed) {
				if (DEBUG_LEVEL > 1) {
					cout << "Improvement to global best solution found!" << endl;
				}
				delete tempImp;
				delete curBest;
				curBest = toImprove;
				toImprove = new Solution(curBest);
				#ifdef ubigraph
				toImprove->redraw();
				#endif
				no_imp_runs = 0;
				shakeSteps = shakeStart - shakeIncrement;
			}
			/// Reset local best solution to global best Solution
			else {
				delete tempImp;
				no_imp_runs++;

				/// Stopping condition, quit VNS
				if (no_imp_runs > unsuccessfulShake) {
					if (DEBUG_LEVEL > 0) {
						cout << "Too many unsuccessful shakes!" << endl;
					}
					break;
				}
			}
			
			int shakeNeighbor = rand() % neighbors.size();
			curNeighbor = 0;
			VNS_Unit *shaker = neighbors[shakeNeighbor];
			toImprove = shaker->shuffleSolution(*toImprove, orig, (shakeSteps += shakeIncrement));
			
			bool checkResult = false;
			if (checkIntermediate) {
				checkResult = checkValid(toImprove, &orig);
				if (!checkResult) {
					delete toImprove;
					toImprove = new Solution(curBest);
				}
			}
			
			if (DEBUG_LEVEL > 1) {
				cout << "Shaking Solution using " << shaker->getName()<< " with ";
				cout << shakeSteps << " steps returned a ";
				
				if (checkIntermediate)
					cout << (checkResult ? "valid" : "invalid");
				else
					cout << "new";

				cout << " solution using " << toImprove->colorsUsed << " colors." << endl;
			}
		}
		bool isValid = false;
		
		if (checkFinal)
			isValid = checkValid(curBest, &orig);
		
		if (DEBUG_LEVEL > 0) {
			cout << "Final best solution uses " << curBest->colorsUsed << " colors";
			
			if (checkFinal)
				cout << " and is " << (isValid ? "valid" : "invalid");
				
			cout << "." << endl;
		}
		/// Print stats
		cout << "{" << endl;
		cout << "  \"units\": \"" << units << "\"," << endl;
		cout << "  \"improvement\" : " << (bestSolution->colorsUsed - curBest->colorsUsed) << "," << endl;
		cout << "  \"colors\" : " << curBest->colorsUsed << "," << endl;
		cout << "  \"instance\" : {" << endl;
		cout << "    \"vertices\": " << num_vertices(*orig.g) << "," << endl;
		cout << "    \"edges\": " << num_edges(*orig.g) << "," << endl;
		cout << "    \"partitions\": " << orig.numParts << endl;
		cout << "  }," << endl;
		
		if (checkFinal)
			cout << "  \"valid\" : " << (isValid ? "true" : "false") << "," << endl;

		cout << "  \"stats\": [" << endl;
	
		for (unsigned int i = 0; i < neighbors.size(); i++) {
			int impCount = 0;
			pair<int, int> all, imp;
			all.first = 0;
			all.second = 0;
			imp.first = 0;
			imp.second = 0;
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
			allDev.first = 0;
			allDev.second = 0;
			impDev.first = 0;
			impDev.second = 0;
			for (vector<pair<int, int>>::iterator j = stats[i].begin(); j < stats[i].end(); j++) {
				allDev.first += pow((double)((*j).first - allAvg.first), 2);
				allDev.second += pow((double)((*j).second - allAvg.second), 2);
				
				if ((*j).second > 0) {
					impDev.first += pow((double)((*j).first - impAvg.first), 2);
					impDev.second += pow((double)((*j).second - impAvg.second), 2);
				}
			}
			allDev.first = sqrt(allDev.first / (double)stats[i].size());
			allDev.second = sqrt(allDev.second / (double)stats[i].size());
			impDev.first = sqrt(impDev.first / (double)impCount);
			impDev.second = sqrt(impDev.second / (double)impCount);
		
			VNS_Unit *cur = neighbors[i];
			
			cout << "    {" << endl;
			cout << "      \"name\" : \"" << cur->getName() << "\"," << endl;
			cout << "      \"abbreviation\" : \"" << cur->getAbbreviation() << "\"," << endl;
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
			cout << "      }";
			if (impCount) {
				cout << "," << endl;
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
			}
			else {
				cout << endl;
			}
			cout << "    }";
			
			if (i != neighbors.size() - 1)
				cout << ",";
				
			cout << endl;
		}
		
		cout << "  ]" << endl;
		cout << "}" << endl;
	
		Solution* res = new Solution(curBest);	
		delete[] stats;
		delete curBest;
		delete bestSolution;
		
		if (curBest != toImprove)
			delete toImprove;
		
		for(vector<VNS_Unit*>::iterator i = neighbors.begin(); i != neighbors.end();
			 i++) {
			delete *i;
		}
		#ifdef ubigraph
		res->redraw();
		#endif
		return res;
	}
	
	/// validate solutions
	bool checkValid(Solution* s, Solution *full) {
		pair<VertexIter, VertexIter> vIter;
		int *parts = new int[s->numParts];
		int *colors = new int[s->numParts];
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
		delete[] parts;
		delete[] colors;
			
		return valid;
	}
}

