#include "../include/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const string changeNode::getName() {
	return "changeNode";
}

const char changeNode::getAbbreviation() {
	return 'n';
}

Solution *changeNode::findLocalMin(Solution& best, Solution& full) {
	Solution* s = &best;
	int maxColor = best.colorsUsed - 1;
	s->requestDeepCopy();
	
	VertexIter i, iEnd;
	vector<Vertex> *candidates;
	
	// Search all vertices for minimal colors
	for (tie(i, iEnd) = vertices(*s->g); i != iEnd; i++) {
		if (s->getPartitionColor(*i) == maxColor) {
			Vertex n = *i;
			
			// Only execute changeNode if there are nodes to replace
			candidates = &(full.partNodes[s->getPartition(n)]);
			if (candidates->size() > 1) {
				vector<Vertex>::iterator r;
				for (r = candidates->begin(); r != candidates->end(); r++) {
					if (*r != s->getOriginalId(n)) {
						s->replaceVertex(n, *r, full);
						
						// Check for improvement
						int color = s->minPossibleColor(n);
						if (color < maxColor) {
							s->setPartitionColor(n, color);
							break;
						}
					}
				}
				// No candidates left to try...
				if (r == candidates->end()) {
					s->colorsUsed = s->numParts;
					#ifdef ubigraph
					s->redraw();
					#endif
					return s;
				}
			}
			else
				return s;
		}
	}
	
	maxColor = 0;
	for (int i = 0; i < s->numParts; i++) {
		
		if (s->partition[i] > maxColor) {
			maxColor = s->partition[i];
		}
	}
	s->colorsUsed = maxColor + 1;
	
	if (DEBUG_LEVEL > 1) {
		cout<<"changeNode uses "<<s->colorsUsed<<" colors"<<endl; 
	}
	
	Solution *temp = new Solution(s);
	temp = this->findLocalMin(*temp, full);
	if (temp->colorsUsed < s->colorsUsed) {
		delete s;
		s = temp;
		#ifdef ubigraph
		s->redraw();
		#endif
	}
	else 
		delete temp;
	
	return s;
}

Solution *changeNode::shuffleSolution(Solution& cur, Solution& full,
				 							  	  int numSteps) {
	Solution* ret = &cur;
	ret->requestDeepCopy();
	
	VertexPart_Map vPartsOrig = get(vertex_index1_t(), *full.g);
	VertexID_Map vIndex = get(vertex_index2_t(), *ret->g);
	pair<AdjIter, AdjIter> ai;
	int *colors = new int[ret->numParts];
	int i = 0;
	
	for (int a = 0; a < numSteps; a++) {
		/// Select a random node in the original graph and replace it in the 
		/// solution graph
		Vertex replacement = rand() % num_vertices(*full.g);
		int part = vPartsOrig[replacement];
		Vertex v = ret->representatives[part];
		
		/// If it isn't already the current chosen node
		if (vIndex[ret->representatives[part]] != replacement) {
			ret->replaceVertex(v, replacement, full);
			ret->setPartitionColor(v, ret->minPossibleColor(v));
		}
	}
	
	for (i = 0; i < ret->numParts; i++) {
				colors[i] = 0;
	}
	
	int maxColor = 0;
	bool gap = false;
	for (i = 0; i < ret->numParts; i++) { 
		colors[ret->partition[i]] = 1;
		if (maxColor < ret->partition[i]) {
			maxColor = ret->partition[i];
		}
	}
	int minGap = 0;
	for (i = 0; i < maxColor + 1; i++) {
		if (colors[i] == 0) {
			gap = true;
			minGap = i;
			break;
		}
	}
	while (gap) {
		gap = false;
		for (i = minGap; i < ret->numParts; i++) {
				colors[i] = 0;
		}
		maxColor = 0;
		for (i = 0; i < ret->numParts; i++) {
			if (ret->partition[i] > minGap) {
				ret->partition[i]--;
				colors[ret->partition[i]] = 1;
				if (maxColor < ret->partition[i]) {
					maxColor = ret->partition[i];
				}
			} 
		}
		for (i = minGap; i < maxColor + 1; i++) {
			if (colors[i] == 0) {
				gap = true;
				minGap = i;
				break;
			}
		}
	}
	delete[] colors;
	ret->colorsUsed = maxColor + 1;
	return ret;
}
