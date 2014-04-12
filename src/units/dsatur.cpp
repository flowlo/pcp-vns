#include "../../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string dsatur::getName() {
	return "dsatur";
}

const char dsatur::getAbbreviation() {
	return 'd';
}

Solution dsatur::findLocalMin(Solution& current) {
	current.clearColors();

	color_t targetColor = -1, minColor = -1;
	Vertex target;
	uint32_t maxColorDegree = 0, colorDegree = 0, maxBlankDegree = 0, blankDegree = 0;
	FVertexIter i, end;

	for (uint32_t j = 0; j < current.getNumPartition(); j++) {
		if (DEBUG_LEVEL == 4)
			cout << "Iteration " << j << " ..." << endl;

		maxColorDegree = maxBlankDegree = 0;

		for (tie(i, end) = vertices(current.getCurrentSolution()); i != end; i++) {
			if (current.isVertexColored(*i))
				continue;

			tie(colorDegree, minColor) = current.getColorDegreeAndMinColor(*i);

			if (colorDegree < maxColorDegree)
				continue;

			blankDegree = out_degree(*i, current.getCurrentSolution()) - colorDegree;

			if (colorDegree == maxColorDegree && blankDegree <= maxBlankDegree)
				continue;

			target = *i;
			maxColorDegree = colorDegree;
			maxBlankDegree = blankDegree;
			targetColor = minColor;

			if (DEBUG_LEVEL == 4)
				cout << "  New best " << target << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
		}
		current.setVertexColor(target, targetColor);

		if (DEBUG_LEVEL == 4)
			cout << "  Colored " << target << " with " << targetColor << endl;
	}
	return current;
}

Solution dsatur::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
