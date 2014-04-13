// This file contains the implementation of VNS
#include "../include/vns-priv.hpp"

using namespace std;
using namespace boost;

namespace pcp {
	bool TERM_SIGINT = false;
	
	void sigint_catch(int signum) {
		TERM_SIGINT = signum == SIGINT;
	}
	
	// Implementation of VNS, see vns.hpp
	Solution vns(Solution& best, std::string units, int unsuccessfulShake, int shakeStart, int shakeIncrement, int maxTime, bool checkIntermediate, bool checkFinal) {
		// Backup the starting solutions
		int initialUsed = best.getColorsUsed();

		signal(SIGINT, sigint_catch);

		if (DEBUG_LEVEL > 1) {
			cout<<"Best solution uses "<<initialUsed<<" colors"<<endl;
			//cout<<"Full solution uses "<<orig.getColorsUsed()<<" colors"<<endl;
		}
		if (DEBUG_LEVEL > 1) {
			cout<<"The supplied solution is valid: "<<(best.isValid().first ? "true": "false")<<endl;
		}

		vector<VNS_Unit*> neighbors = vector<VNS_Unit*>();

		for (string::iterator i = units.begin(); i < units.end(); ++i) {
			if (*i == changeNode::getAbbreviation())
				neighbors.push_back(new changeNode());
			else if (*i == changeColor::getAbbreviation())
				neighbors.push_back(new changeColor());
			/*else if (*i == tabuSearch::getAbbreviation())
				neighbors.push_back(new tabuSearch());*/
			else if (*i == dsatur::getAbbreviation())
				neighbors.push_back(new dsatur());
			else if (*i == changeAll::getAbbreviation())
				neighbors.push_back(new changeAll());
			else {
				cerr << "Invalid unit specified. " << units << endl;
				return Solution();
			}
		}

		vector<pair<int, int>> *stats = new vector<pair<int, int>>[neighbors.size()];

		time_t startTime = time(NULL);
		int no_imp_runs = 0, improvement;
		unsigned int curNeighbor = 0;
		int shakeSteps = shakeStart - shakeIncrement;
		Solution toImprove = best;
		Solution &curBest = best;
		pair<bool,string> checkResult(false,"");

		unordered_set<condensate> solutionStore;

		// Run as long as shaking still produces usefull solution
		while (!TERM_SIGINT) {

			// No time left, abort main loop
			if (startTime + maxTime < time(NULL)) {
				if (DEBUG_LEVEL > 0) {
					cout << "Time's up! Aborting VNS ..." << endl;
				}
				break;
			}

			Solution tempImp(toImprove);

			// Run all possible neighborhood
			while (curNeighbor < neighbors.size() && !TERM_SIGINT) {

				// Select the new neighborhood
				clock_t start = clock();
				VNS_Unit *neigh = neighbors[curNeighbor];

				if (DEBUG_LEVEL > 1) {
					cout << "Running " << neigh->getName() << " ..." << endl;
				}

				// Compute the minimum for this neighborhood
				Solution imp(tempImp);
				neigh->findLocalMin(imp);	

				improvement = (toImprove < imp) ? 0 : toImprove.getColorsUsed() - imp.getColorsUsed();

				// Stats tracking
				stats[curNeighbor].push_back(pair<int, int>(clock() - start, improvement));

				if (DEBUG_LEVEL > 1) {
					cout<<neigh->getName()<<" took about ";
					cout<<(clock() - start)/(float)CLOCKS_PER_SEC;
					cout<<" seconds to complete."<<endl;
				}
				// Replace the existing solution with the new solution if it is an
				// improvement
				if (improvement > 0) {
					//TODO
					//if (toImprove != curBest)
					//	delete toImprove;

					// TODO
					toImprove = tempImp = imp;
					//tempImp = new Solution(tempImp);

					condensate check = imp.condense();
					if (solutionStore.find(check) != solutionStore.end()) {
						if (DEBUG_LEVEL >= 3)
							cout << "Detected duplicate solution, proceeding to shake!" << endl;

						curNeighbor = 0;
						break;
					}
					else {
						solutionStore.insert(check);
						if (DEBUG_LEVEL >= 3)
							cout << "Added new solution to set." << endl;
					}

					if (DEBUG_LEVEL > 1) {
						cout << "Found new local minimum with " << neigh->getName() << " using " << tempImp.getColorsUsed() << " colors.";

						if (checkIntermediate) {
							checkResult = tempImp.isValid();

							if (checkResult.first) {
								cout << "The new solution is valid." << endl;
							} else {
								cout << "The new solution is invalid (" << checkResult.second << ")." << endl;
							}
						}
					}

					// Restart neighborhoods
					curNeighbor = curNeighbor == 0 ? 1 : 0;
				}

				// Step to next neighborhood, no improvement found
				else {
					if (DEBUG_LEVEL > 1)
						cout << "No improvement found." << endl;

					if (toImprove > imp) {
						//delete tempImp;
						tempImp = toImprove;
					}

					curNeighbor++;
				}
			} // end while neighborhood

			if (DEBUG_LEVEL > 1) {
				cout << "End of inner VNS loop." << endl;
			}

			// Local minimum of neighborhoods is better than current best
			// solution
			if (toImprove < curBest) {
				if (DEBUG_LEVEL > 1) {
					cout << "Improvement to global best solution found!" << endl;
				}
				//delete tempImp;
				//delete curBest;
				curBest = toImprove;
				//toImprove = new Solution(curBest);
				#ifdef ubigraph
				toImprove.redraw();
				#endif
				no_imp_runs = 0;
				shakeSteps = shakeStart - shakeIncrement;
			}
			// Reset local best solution to global best Solution
			else {
				//delete tempImp;
				no_imp_runs++;

				// Stopping condition, quit VNS
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
			toImprove = shaker->shuffleSolution(toImprove, (shakeSteps += shakeIncrement));

			if (checkIntermediate) {
				checkResult = toImprove.isValid();
				if (!checkResult.first) {
					//delete toImprove;
					toImprove = Solution(curBest);
				}
			}

			if (DEBUG_LEVEL > 1) {
				cout << "Shaking Solution using " << shaker->getName()<< " with ";
				cout << shakeSteps << " steps returned ";

				if (checkIntermediate)
					cout << (checkResult.first ? "a valid" : "an invalid");
				else
					cout << "a new";

				cout << " solution using " << toImprove.getColorsUsed() << " colors." << endl;
				if (checkIntermediate && !checkResult.first)
					cout << "Reason: " << checkResult.second << endl;
			}
		}
		bool isValid = false;

		if (checkFinal)
			isValid = curBest.isValid().first;

		if (DEBUG_LEVEL > 0) {
			cout << "Final best solution uses " << curBest.getColorsUsed() << " colors";

			if (checkFinal)
				cout << " and is " << (isValid ? "valid" : "invalid");

			cout << "." << endl;
		}
		// Print stats
		cout << "{" << endl;
		cout << "  \"units\": \"" << units << "\"," << endl;
		cout << "  \"improvement\" : " << (initialUsed - curBest.getColorsUsed()) << "," << endl;
		cout << "  \"colors\" : " << curBest.getColorsUsed() << "," << endl;
		cout << "  \"instance\" : {" << endl;
		cout << "    \"vertices\": " << curBest.getNumVertices() << "," << endl;
		cout << "    \"edges\": " << curBest.getNumEdges() << "," << endl;
		cout << "    \"partitions\": " << curBest.getNumPartition() << endl;
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
			for (vector<pair<int, int>>::iterator j = stats[i].begin(); j < stats[i].end(); ++j) {
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
			for (vector<pair<int, int>>::iterator j = stats[i].begin(); j < stats[i].end(); ++j) {
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
	
		delete[] stats;
		//delete curBest;
		//delete initial;

		//TODO
		//if (curBest != toImprove)
		//	delete toImprove;

		for(vector<VNS_Unit*>::iterator i = neighbors.begin(); i != neighbors.end(); ++i) {
			delete *i;
		}
		#ifdef ubigraph
		curBest.redraw();
		#endif

		return best;
	}
}

