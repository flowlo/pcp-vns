#include "../header/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string dsatur::getName() {
	return "dsatur";
}

const char dsatur::getAbbreviation() {
	return getStaticAbbreviation();
}

const char dsatur::getStaticAbbreviation() {
	return 'd';
}

/// Compute the new improved solution of this neighborhood
Solution* dsatur::findLocalMin(Solution& curBest, Solution& full) {
	Solution* s = &curBest;
	
	fill(s->partition, s->partition + s->numParts, -1);
	
	int numColors = -1, target, maxColorDegree, maxBlankDegree, colorDegree, blankDegree, color;
	
	VertexIter i, end;
	
	for (int j = 0; j < s->numParts; j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		target = maxColorDegree = maxBlankDegree = -1;

	 	for (tie(i, end) = vertices(*s->g); i != end; i++) {
	 		if (s->isPartitionColored(*i))
	 			continue;

			colorDegree = s->getColorDegree(*i);

			if (colorDegree < maxColorDegree)
				continue;

			blankDegree = degree(*i, *s->g) - colorDegree;

			if (colorDegree == maxColorDegree && blankDegree <= maxBlankDegree)
				continue;

			target = *i;
			maxColorDegree = colorDegree;
			maxBlankDegree = blankDegree;
			
			if (DEBUG_LEVEL == 4)
				cout << "New best " << target << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
		}
		
		color = s->minPossibleColor(target);
		s->setPartitionColor(target, color);
		
		if (DEBUG_LEVEL == 4)
			cout << "Colored " << target << " with " << color << endl;
		
		if (numColors < color)
			numColors = color;
	}

	s->colorsUsed = numColors + 1;
	return s;
}

/// Shuffle a solution using the neighborhood as a base
Solution* dsatur::shuffleSolution(Solution& cur, Solution& full, int numSteps) {
	Solution* s = &cur;
	
	fill(s->partition, s->partition + s->numParts, -1);
	
	int numColors = -1, target, maxColorDegree, maxBlankDegree, colorDegree, blankDegree, color;
	
	VertexIter i, end;
	
	for (int j = 0; j < s->numParts; j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		target = maxColorDegree = maxBlankDegree = -1;
		
	 	for (tie(i, end) = vertices(*s->g); i != end; i++) {
	 		if (s->isPartitionColored(*i))
	 			continue;
	 		
			colorDegree = s->getColorDegree(*i);
			
			if (rand() % 2)
				if (colorDegree < maxColorDegree)
					continue;
			
			blankDegree = degree(*i, *s->g) - colorDegree;
			
			if (rand() % 3)
				if (colorDegree == maxColorDegree && blankDegree <= maxBlankDegree)
					continue;
				
			target = *i;
			maxColorDegree = colorDegree;
			maxBlankDegree = blankDegree;
			
			if (DEBUG_LEVEL == 4)
				cout << "New best " << target << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
		}
		
		color = s->minPossibleColor(target);
		s->setPartitionColor(target, color);
		
		if (DEBUG_LEVEL == 4)
			cout << "Colored " << target << " with " << color << endl;
		
		if (numColors < color)
			numColors = color;
	}

	s->colorsUsed = numColors + 1;
	return s;
}
