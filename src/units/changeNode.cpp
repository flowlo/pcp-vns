#include "../../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string changeNode::getName() {
	return "changeNode";
}

const char changeNode::getAbbreviation() {
	return 'n';
}

Solution changeNode::findLocalMin(Solution& solution) {
	color_t maxColor = solution.getColorsUsed() - 1;

	FVertexIter i, end;
	vector<Vertex> candidates;

	// Search all vertices for minimal colors
	for (tie(i, end) = vertices(solution.getCurrentSolution()); i != end; ++i) {
		if (solution.getColor(*i) == maxColor) {
			// Only execute changeNode if there are nodes to replace
			candidates = solution.getPartitionNodes(solution.getPartition(*i));
			if (candidates.size() > 1) {
				vector<Vertex>::iterator replacement;
				for (replacement = candidates.begin(); replacement != candidates.end(); ++replacement) {
					if (*replacement != *i) {
						solution.replaceVertex(*i, *replacement);

						// Check for improvement
						color_t color = solution.minPossibleColor(*replacement);
						if (color < maxColor) {
							solution.setVertexColor(*replacement, color);
							break;
						}
						else
							solution.replaceVertex(*replacement, *i);
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
		cout<<"changeNode uses "<<solution.getColorsUsed()<<" colors"<<endl; 
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

Solution changeNode::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
