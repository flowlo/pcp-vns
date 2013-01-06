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
	typename boost::graph_traits<Graph>::adjacency_iterator adj_i, adj_end;
	
	VertexPart_Map vParts = get(vertex_index1_t(), *s.g);
	
	int colored = 0;

	for (tie(adj_i, adj_end) = adjacent_vertices(node, *s.g); adj_i != adj_end; adj_i++)
		if (s.partition[get(vParts, *adj_i)] != -1)
			colored++;

	return colored;
}

int getBlankDegree(Vertex node, Solution& s) {
	typename boost::graph_traits<Graph>::adjacency_iterator adj_i, adj_end;
	
	VertexPart_Map vParts = get(vertex_index1_t(), *s.g);
	
	int colored = 0;

	for (tie(adj_i, adj_end) = adjacent_vertices(node, *s.g); adj_i != adj_end; adj_i++)
		if (s.partition[get(vParts, *adj_i)] == -1)
			colored++;

	return colored;
}

int minPossibleColor(Vertex node, Solution& s) {
	VertexPart_Map vParts = get(vertex_index1_t(), *s.g);
	
	int colors[s.numParts];

	for (int i = 0; i < s.numParts; i++)
		colors[i] = 0;

	typename boost::graph_traits<Graph>::adjacency_iterator iter, last;

	for (tie(iter, last) = adjacent_vertices(node, *s.g); iter != last; ++iter)
		if (s.partition[get(vParts, *iter)] != -1)
			colors[s.partition[get(vParts, *iter)]] = 1;

	for (int i = 0; i < s.numParts; i++)
		if (colors[i] == 0)
			return i;

	return -1;
}


/// Compute the new improved solution of this neighborhood
Solution* dsatur::findLocalMin(Solution& curBest, Solution& full) {
	Solution* s = new Solution(&curBest);
	
	std::fill(s->partition, s->partition + s->numParts, -1);
	
	VertexPart_Map vParts = get(vertex_index1_t(), *s->g);
	
	int numColors = -1, candidate, maxColorDegree, maxBlankDegree, colorDegree, blankDegree;
	
	std::pair<VertexIter, VertexIter> vp;
	
	for (int j = 0; j < s->numParts; j++) {
		if (DEBUG_LEVEL == 4)
			cout << "New iteration!" << endl;

		candidate = -1;
		maxColorDegree = 0;
		maxBlankDegree = 0;
		
	 	for (vp = vertices(*s->g); vp.first != vp.second; ++vp.first) {
	 		if (s->partition[get(vParts, *vp.first)] != -1) {
	 			if (DEBUG_LEVEL == 4)
		 			cout << "Skipping " << *vp.first << endl;
	 			continue;
	 		}
	 		
			colorDegree = getColorDegree(*vp.first, *s);
			
			if (colorDegree > maxColorDegree) {
				candidate = *vp.first;
				maxColorDegree = colorDegree;
				maxBlankDegree = getBlankDegree(*vp.first, *s);
				
				if (DEBUG_LEVEL == 4)
					cout << "New best " << candidate << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
			}
			else if (colorDegree == maxColorDegree) {
				blankDegree = getBlankDegree(*vp.first, *s);
				if (maxBlankDegree < blankDegree) {
					candidate = *vp.first;
					maxColorDegree = colorDegree;
					maxBlankDegree = blankDegree;
					
					if (DEBUG_LEVEL == 4)
						cout << "New best " << candidate << " having (" << maxColorDegree << "|" << maxBlankDegree << ")" << endl;
				}
			}
		}
		
		int color = minPossibleColor(candidate, *s);
		s->partition[get(vParts, candidate)] = color;
		
		if (DEBUG_LEVEL == 4)
			cout << "Colored " << candidate << " with " << color << endl;
		
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
