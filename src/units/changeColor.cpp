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
	color_t maxColor = solution.getColorsUsed() - 1;
	FVertexIter i, iEnd;
	FAdjIter a, aEnd;

	// Change color will not work here....
	if (maxColor < 2)
		return solution;

	if (DEBUG_LEVEL > 2)
		cout<<"Starting changeColor"<<endl;

	// Try recoloring all vertices where color=maxColor
	for (tie(i, iEnd) = vertices(solution.getCurrentSolution()); i != iEnd; i++) {
		if (solution.getVertexColor(*i) == maxColor) {
			if (DEBUG_LEVEL > 3) {
				cout<<"vertex "<<*i<<"has maxColor; ";
			}

			// Try recoloring the vertex with every possible color < maxColor - 1
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
			// No new color could be found for all adjacent vertices
			if (j == maxColor) {
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
	for (partition_t i = 0; i < solution.getNumPartition(); i++)
		if (solution.getPartitionColor(i) > maxColor)
			maxColor = solution.getPartitionColor(i);

	solution.setColorsUsed(maxColor + 1);

	if (DEBUG_LEVEL > 1)
		cout<<"changeColor uses "<<solution.getColorsUsed()<<" colors"<<endl; 

	// Try to take it one step further
	/*Solution temp(solution);
	temp = this->findLocalMin(temp);
	if (temp < solution) {
		solution = temp;
		#ifdef ubigraph
		solution.redraw();
		#endif
	}*/

	return solution;
}

Solution changeColor::shuffleSolution(Solution& cur, int numSteps) {
	if (DEBUG_LEVEL > 3) {
		cout<<"Shaking with changeColor"<<endl;
	}
	FVertex rep[cur.getNumPartition()];
	FVertexIter fi, fend;
	for (tie(fi,fend) = vertices(cur.getCurrentSolution()); fi != fend; ++fi) {
		rep[cur.getPartition(*fi)] = *fi;
	}

	// recolor randomly selected nodes
	FVertex node;
	color_t color, maxcolor = -1;
	for (int i = 0; i < numSteps; i++) {
		node = rep[rand() % cur.getNumPartition()];
		color = cur.minPossibleColor(node);
		cur.setVertexColor(node, color);		
		if (color > maxcolor)
			maxcolor = color;
	}
	cur.setColorsUsed(maxcolor + 1);

	return cur;
}
