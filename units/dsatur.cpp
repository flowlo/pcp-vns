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
	
	pair<VertexIter, VertexIter> v;
	
	for (int j = 0; j < s->numParts; j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		target = maxColorDegree = maxBlankDegree = -1;
		
	 	for (v = vertices(*s->g); v.first != v.second; ++v.first) {
	 		if (s->isPartitionColored(*v.first)) {
	 			if (DEBUG_LEVEL == 4)
		 			cout << "Skipping " << *v.first << endl;
	 			continue;
	 		}
	 		
			colorDegree = s->getColorDegree(*v.first);
			
			if (colorDegree < maxColorDegree)
				continue;
			
			blankDegree = degree(*v.first, *s->g) - colorDegree;
			
			if (colorDegree == maxColorDegree && blankDegree <= maxBlankDegree)
				continue;
				
			target = *v.first;
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
	
	pair<VertexIter, VertexIter> v;
	
	for (int j = 0; j < s->numParts; j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		target = maxColorDegree = maxBlankDegree = -1;
		
	 	for (v = vertices(*s->g); v.first != v.second; ++v.first) {
	 		if (s->isPartitionColored(*v.first)) {
	 			if (DEBUG_LEVEL == 4)
		 			cout << "Skipping " << *v.first << endl;
	 			continue;
	 		}
	 		
			colorDegree = s->getColorDegree(*v.first);
			
			if (rand() % 2)
				if (colorDegree < maxColorDegree)
					continue;
			
			blankDegree = degree(*v.first, *s->g) - colorDegree;
			
			if (rand() % 3)
				if (colorDegree == maxColorDegree && blankDegree <= maxBlankDegree)
					continue;
				
			target = *v.first;
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
