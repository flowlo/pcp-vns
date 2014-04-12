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

	// Search all vertices for minimal colors
	for (tie(i, end) = vertices(solution.getCurrentSolution()); i != end; ++i) {
		if (solution.getVertexColor(*i) == maxColor) {
			// Only execute changeNode if there are nodes to replace
			vector<Vertex> candidates = solution.getPartitionNodes(solution.getPartition(*i));
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
	FVertex rep[cur.getNumPartition()];
	FVertexIter fi, fend;
	for (tie(fi,fend) = vertices(cur.getCurrentSolution()); fi != fend; ++fi) {
		rep[cur.getPartition(*fi)] = *fi;
	}
	
	for (int i = 0; i < numSteps; ++i) {
		partition_t part = rand() % cur.getNumPartition();
		cur.setPartitionColor(part, -1);
		vector<Vertex> cand = cur.getPartitionNodes(part);
		Vertex replace = cand[rand() % cand.size()];
		cur.replaceVertex(rep[part], replace);
	}
	
	color_t maxcolor = -1;
	for (tie(fi,fend) = vertices(cur.getCurrentSolution()); fi != fend; ++fi) {
		color_t c = cur.minPossibleColor(*fi);
		if (c > maxcolor)
			c = maxcolor;
		cur.setVertexColor(*fi, c);
	}
	cur.setColorsUsed(maxcolor+1);
	return cur;
}
