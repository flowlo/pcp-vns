#include "../include/vns-priv.hpp"

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
	
	int numColors = -1, target, maxColorDegree, maxBlankDegree, colorDegree, blankDegree, color, targetColor;
	
	VertexIter i, end;
	AdjIter k, last;

	vector<bool> colors = vector<bool>(s->numParts, false);
	
	for (int j = 0; j < s->numParts; j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		target = maxColorDegree = maxBlankDegree = -1;

	 	for (tie(i, end) = vertices(*s->g); i != end; i++) {
	 		if (s->isPartitionColored(*i))
	 			continue;
	 			
	 		fill(colors.begin(), colors.begin() + s->numParts, false);
			
			for (tie(k, last) = adjacent_vertices(*i, *s->g); k != last; k++)
				if (s->isPartitionColored(*k))
					colors[s->partition[s->getPartition(*k)]] = true;

			color = -1;
			colorDegree = 0;
			for (vector<bool>::iterator o = colors.begin(); o < colors.end(); o++)
				if (*o)
					colorDegree++;
				else if (color < 0)
					color = distance(colors.begin(), o);

			if (colorDegree < maxColorDegree)
				continue;

			blankDegree = degree(*i, *s->g) - colorDegree;

			if (colorDegree == maxColorDegree && blankDegree <= maxBlankDegree)
				continue;

			target = *i;
			maxColorDegree = colorDegree;
			maxBlankDegree = blankDegree;
			targetColor = color;
			
			if (DEBUG_LEVEL == 4)
				cout << "New best " << target << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
		}

		s->setPartitionColor(target, targetColor);
		
		if (DEBUG_LEVEL == 4)
			cout << "Colored " << target << " with " << targetColor << endl;
		
		if (numColors < targetColor)
			numColors = targetColor;
	}
	s->colorsUsed = numColors + 1;
	return s;
}

/// Shuffle a solution using the neighborhood as a base
Solution* dsatur::shuffleSolution(Solution& cur, Solution& full, int numSteps) {
	Solution* s = &cur;
	
	fill(s->partition, s->partition + s->numParts, -1);
	
	int numColors = -1, target, maxColorDegree, maxBlankDegree, colorDegree, blankDegree, color, targetColor;
	
	VertexIter i, end;
	AdjIter k, last;

	vector<bool> colors = vector<bool>(s->numParts, false);
	
	for (int j = 0; j < s->numParts; j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		target = maxColorDegree = maxBlankDegree = -1;
		
	 	for (tie(i, end) = vertices(*s->g); i != end; i++) {
	 		if (s->isPartitionColored(*i))
	 			continue;
	 		
	 		fill(colors.begin(), colors.begin() + s->numParts, false);
			
			for (tie(k, last) = adjacent_vertices(*i, *s->g); k != last; k++)
				if (s->isPartitionColored(*k))
					colors[s->partition[s->getPartition(*k)]] = true;

			color = -1;
			colorDegree = 0;
			for (vector<bool>::iterator o = colors.begin(); o < colors.end(); o++)
				if (*o)
					colorDegree++;
				else if (color < 0)
					color = distance(colors.begin(), o);
			
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
			targetColor = color;
			
			if (DEBUG_LEVEL == 4)
				cout << "New best " << target << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
		}
		
		s->setPartitionColor(target, targetColor);
		
		if (DEBUG_LEVEL == 4)
			cout << "Colored " << target << " with " << targetColor << endl;
		
		if (numColors < targetColor)
			numColors = targetColor;
	}

	s->colorsUsed = numColors + 1;
	return s;
}
