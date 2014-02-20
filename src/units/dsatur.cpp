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

	color_t targetColor = -1;
	Vertex target;
	uint32_t maxColorDegree, maxBlankDegree, blankDegree;
	FVertexIter i, end;
	pair<uint32_t, color_t> candidate;

	for (uint32_t j = 0; j < current.getNumPartition(); j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		target = maxColorDegree = maxBlankDegree = -1;

		for (tie(i, end) = vertices(current.getCurrentSolution()); i != end; i++) {
			if (current.isColored(*i))
				continue;

			candidate = current.getColorDegreeAndMinColor(*i);

			if (candidate.first < maxColorDegree)
				continue;

			blankDegree = degree(*i, current.getFullGraph()) - candidate.first;

			if (candidate.first == maxColorDegree && blankDegree <= maxBlankDegree)
				continue;

			target = *i;
			maxColorDegree = candidate.first;
			maxBlankDegree = blankDegree;
			targetColor = candidate.second;

			if (DEBUG_LEVEL == 4)
				cout << "New best " << target << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
		}
		current.setVertexColor(target, targetColor);

		if (DEBUG_LEVEL == 4)
			cout << "Colored " << target << " with " << targetColor << endl;
	}
	return current;
}

Solution dsatur::shuffleSolution(Solution& cur, int numSteps) {
	return cur;
}
