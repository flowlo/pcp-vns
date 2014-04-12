#include "../../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string changeAll::getName() {
	return "changeAll";
}

const char changeAll::getAbbreviation() {
	return 'a';
}

Solution changeAll::findLocalMin(Solution& solution) {
	color_t maxColor = solution.getColorsUsed() - 1;

	FVertexIter i, end;
	FAdjIter a, aEnd;

	// Search all vertices for max colors
	for (tie(i, end) = vertices(solution.getCurrentSolution()); i != end; ++i) {
		if (solution.getVertexColor(*i) == maxColor) {
			// Only execute changeNode if there are nodes to replace
			vector<Vertex> candidates = solution.getPartitionNodes(solution.getPartition(*i));
			if (candidates.size() > 1) {
				vector<Vertex>::iterator replacement;
				for (replacement = candidates.begin(); replacement != candidates.end(); ++replacement) {
					if (*replacement != *i) {
						solution.replaceVertex(*i, *replacement);

						// Try all colors
						color_t j;
						for(j = 0; j < maxColor; ++j)  {
							solution.setVertexColor(*i, j);

							if (DEBUG_LEVEL > 3) {
								cout<<"new color is "<<j<<endl;
							}

							// Try recoloring adjacent vertices with a color smaller than 
							// maxColor
							bool allColored = true;
							for (tie(a, aEnd) = adjacent_vertices(*i, solution.getCurrentSolution()); a != aEnd; a++) {
								if (solution.getVertexColor(*a) == j) {
									color_t recolor = solution.minPossibleColor(*a);
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
						if (j == maxColor) {
							solution.replaceVertex(*replacement, *i);
						}
						else {
							break;
						}
					}
				}
				// No candidates left to try...
				if (replacement == candidates.end()) {
					solution.setColorsUsed(solution.getNumPartition());
					#ifdef ubigraph
					solution.redraw();
					#endif
					return solution;
				}
			}
			else
				return solution;
		}
	}

	maxColor = 0;
	for (partition_t i = 0; i < solution.getNumPartition(); i++)
		if (solution.getPartitionColor(i) > maxColor)
			maxColor = solution.getPartitionColor(i);

	solution.setColorsUsed(maxColor + 1);

	if (DEBUG_LEVEL > 1) {
		cout<<"changeAll uses "<<solution.getColorsUsed()<<" colors"<<endl; 
	}

	/*Solution temp = this->findLocalMin(solution);
	if (temp < solution) {
		solution = temp;
		#ifdef ubigraph
		solution.redraw();
		#endif
	}*/

	return solution;
}

Solution changeAll::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
