#include "../header/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const char *changeNode::name() {
	return "changeNode";
}

Solution *changeNode::findLocalMin(Solution& best, Solution& full) {
	Solution* s = new Solution(best);
	int maxColor = best.colorsUsed - 1;
	const int ITER_MAX = 100;
	pair<VertexIter, VertexIter> vIter;
	VertexPart_Map vParts = get(vertex_index1_t(), *s->g);
	VertexPart_Map vPartsOrig = get(vertex_index1_t(), *full.g);
   VertexID_Map vIndex = get(vertex_index2_t(), *s->g);
   
	cout<<"Starting changeNode"<<endl;
	
	/// Recolor all nodes who use maxColor with a random color < maxColor
	for (vIter = vertices(*s->g); vIter.first != vIter.second; vIter.first++) {
		if (s->partition[vParts[*vIter.first]] == maxColor) {
			int col = rand() % maxColor;
			cout<<"Recoloring "<<*vIter.first<<" with color ";
			cout<<col<<endl;
			s->partition[vParts[*vIter.first]] = col;
		}
	}
	
	typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
	pair<AdjIter, AdjIter> ai;
	int colors[s->numParts];
	int i = 0;
	int iter = 0;
	std::vector<Vertex> conflicts(2);
	std::vector<Vertex> partition(2);
	//conflicts.clear();
	
	/// Add all nodes of a conflict to vector conflict
	for (vIter = vertices(*s->g); vIter.first != vIter.second; 
			  vIter.first++) {
			
		for (ai = adjacent_vertices(*vIter.first, *s->g); 
			  ai.first != ai.second; ai.first++) {
			
			if (s->partition[vParts[*ai.first]] ==
				 s->partition[vParts[*vIter.first]]) {
				
				cout<<"Conflicting node "<<*vIter.first<< " with color ";
				cout<<s->partition[vParts[*vIter.first]]<<" added to conflicts";
				cout<<endl;
				conflicts.push_back(*vIter.first);
				break;
			}
		}
	}
	
	/// Run until there are no more conflicts, or there were to many iterations
	while (conflicts.size() != 0 && iter < ITER_MAX) {
		iter++;
		
		/// Pick a random conflicting partition
		int random = rand() % conflicts.size();
		Vertex v = conflicts[random];
		partition.clear();
		int part = vParts[v];
		
		/// Find all vertices of the chosen partition
		for (vIter = vertices(*full.g); vIter.first != vIter.second; 
			  vIter.first++) {
			if (vPartsOrig[*vIter.first] == part && *vIter.first != vIndex[v]) {
				partition.push_back(*vIter.first);
			}
		}
		
		
	}
	
	
	return s;
}

Solution *changeNode::shuffleSolution(Solution& cur, Solution& full,
				 							  int numSteps) {
	return new Solution(&cur);
}
