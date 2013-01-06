#include "../header/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

/// Returns a given name for the neighborhood
const char* dsatur::name() {
	return "DSATUR";
}

/// Returns a given (unique) character used to quickly reference
/// an unit via command line argument.
const char dsatur::abbreviation() {
	return 'd';
}

int getColorDegree(Vertex node, Solution& s) {
	typename boost::graph_traits<Graph>::adjacency_iterator i, end;
	
	int colored = 0;

	for (tie(i, end) = adjacent_vertices(node, *s.g); i != end; i++)
		if (s.isPartitionColored(*i))
			colored++;

	return colored;
}

int minPossibleColor(Vertex node, Solution& s) {
	int colors[s.numParts];

	for (int i = 0; i < s.numParts; i++)
		colors[i] = 0;

	typename boost::graph_traits<Graph>::adjacency_iterator iter, last;

	for (tie(iter, last) = adjacent_vertices(node, *s.g); iter != last; ++iter)
		if (s.isPartitionColored(*iter))
			colors[s.partition[s.getPartition(*iter)]] = 1;

	for (int i = 0; i < s.numParts; i++)
		if (colors[i] == 0)
			return i;

	return -1;
}

/// Compute the new improved solution of this neighborhood
Solution* dsatur::findLocalMin(Solution& curBest, Solution& full) {
	Solution* s = new Solution(&curBest);
	
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
	 		
			colorDegree = getColorDegree(*v.first, *s);
			
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
		
		color = minPossibleColor(target, *s);
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
	return &cur;
}
