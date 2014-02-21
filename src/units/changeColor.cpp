#include "../../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string changeColor::getName() {
	return "changeColor";
}

const char changeColor::getAbbreviation() {
	return 'c';
}

Solution changeColor::findLocalMin(Solution& solution) {
	int maxColor = solution.getColorsUsed() - 1;
	FVertexIter i, iEnd;
	FAdjIter a, aEnd;

	// Change color will not work here....
	if (maxColor <= 1) {
		return solution;
	}

	if (DEBUG_LEVEL > 2) {
		cout<<"Starting changeColor"<<endl;
	}

	// Try recoloring all vertices where color=maxColor
	for (tie(i, iEnd) = vertices(solution.getCurrentSolution()); i != iEnd; i++) {
		if (solution.getColor(*i) == maxColor) {
			if (DEBUG_LEVEL > 3) {
				cout<<"vertex "<<*i<<"has maxColor; ";
			}

			// Try recoloring the vertex with every possible color < maxColor - 1
			int j;
			for(j = 0; j < maxColor - 1; ++j)  {
				solution.setVertexColor(*i, j);

				if (DEBUG_LEVEL > 3) {
					cout<<"new color is "<<j<<endl;
				}

				// Try recoloring adjacent vertices with a color smaller than 
				// maxColor
				bool allColored = true;
				for (tie(a, aEnd) = adjacent_vertices(*i, solution.getCurrentSolution()); a != aEnd; a++) {
					if (solution.getPartitionColor(*a) == j) {
						int recolor = solution.minPossibleColor(*a);
						if (recolor >= maxColor) {
							allColored = false;
							break;
						}
						else {
							solution.setVertexColor(*a, recolor);
						}
					}
				}
				if (allColored)
					break;
			}
			// No new color could be found for all adjacent vertices
			if (j == maxColor - 1) {
				if (DEBUG_LEVEL > 1) {
					cout << "Conflict found: could not resolve conflict with node " << *i <<endl;
				}
				solution.setColorsUsed(solution.getNumPartition());
				return solution;
			}
		}
	}

	// Find new maxColor and number of used colors
	maxColor = 0;
	for (uint32_t i = 0; i < solution.getNumPartition(); i++) {
		if (solution.getPartitionColor(i) > maxColor) {
			maxColor = solution.getPartitionColor(i);
		}
	}
	solution.setColorsUsed(maxColor + 1);

	if (DEBUG_LEVEL > 1) {
		cout<<"changeColor uses "<<solution.getColorsUsed()<<" colors"<<endl; 
	}

	// Try to take it one step further
	Solution temp = this->findLocalMin(solution);
	if (temp.getColorsUsed() < solution.getColorsUsed()) {
		solution = temp;
		#ifdef ubigraph
		solution.redraw();
		#endif
	}

	return solution;
}

Solution changeColor::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
