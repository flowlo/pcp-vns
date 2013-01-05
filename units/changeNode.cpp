#include "../header/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const char *changeNode::name() {
	return "changeNode";
}

const char changeNode::abbreviation() {
	return 'n';
}

Solution *changeNode::findLocalMin(Solution& best, Solution& full) {
	Solution* s = new Solution(&best);
	int maxColor = best.colorsUsed - 1;
	const int ITER_MAX = 1000;
	pair<VertexIter, VertexIter> vIter;
	VertexPart_Map vParts = get(vertex_index1_t(), *s->g);
	VertexPart_Map vPartsOrig = get(vertex_index1_t(), *full.g);
   VertexID_Map vIndex = get(vertex_index2_t(), *s->g);
   
   if (DEBUG_LEVEL > 2) {
		cout<<"Starting changeNode"<<endl;
	}
	
	/// Recolor all nodes who use maxColor with a random color < maxColor
	for (vIter = vertices(*s->g); vIter.first != vIter.second; vIter.first++) {
		if (s->partition[vParts[*vIter.first]] == maxColor) {
			int col = rand() % maxColor;
			s->partition[vParts[*vIter.first]] = col;
			if (DEBUG_LEVEL > 3) {		
				cout<<"Recoloring "<<*vIter.first<<" with color ";
				cout<<col<<endl;
			}
		}
	}
	
	typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
	pair<AdjIter, AdjIter> ai;
	int colors[s->numParts];
	int i = 0;
	int iter = 0;
	std::vector<Vertex> conflicts;
	std::vector<Vertex> partition;
	
	/// Add all nodes of a conflict to vector conflict
	for (vIter = vertices(*s->g); vIter.first != vIter.second; 
			  vIter.first++) {
			
		for (ai = adjacent_vertices(*vIter.first, *s->g); 
			  ai.first != ai.second; ai.first++) {
			
			if (s->partition[vParts[*ai.first]] ==
				 s->partition[vParts[*vIter.first]]) {
				
				if (DEBUG_LEVEL > 3) {
					cout<<"Conflicting node "<<*vIter.first<< " with color ";
					cout<<s->partition[vParts[*vIter.first]]<<" added to conflicts";
					cout<<endl;
				}
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
		
		if (DEBUG_LEVEL > 3) {
			cout<<"Chose random Vertex "<<v<<" in partition "<<part;
			cout<<" from conflicts, this vertex is equal to vertex ";
			cout<<vIndex[v]<<" in the original graph"<<endl;
		}
		
		/// Find all vertices of the chosen partition
		for (vIter = vertices(*full.g); vIter.first != vIter.second; 
			  vIter.first++) {
			if (vPartsOrig[*vIter.first] == part && *vIter.first != vIndex[v]) {
				if (DEBUG_LEVEL > 3) {
					cout<<"Found node "<<*vIter.first<<" in partition "<<part<<endl;
				}
				partition.push_back(*vIter.first);
			}
		}
		
		/// If partition contains other vertices
		if (partition.size() != 0) {
		
			/// Choose random replacement node
			random = rand() % partition.size();
			Vertex rep = partition[random];
			if (DEBUG_LEVEL > 3) {
				cout<<"Randomly chosen replacement is "<<rep<<endl;
			}
			
			/// Clear old vertex of all edges
			clear_vertex(v, *s->g);	
			
			for (i = 0; i < s->numParts; i++) {
				colors[i] = 0;
			}
			
			/// Search matching edges for replacement vertex
			i = 0;
			for (ai = adjacent_vertices(rep, *full.g); 
				  ai.first != ai.second; ai.first++) {
				  
				if (vIndex[s->representatives[vPartsOrig[*ai.first]]] == *ai.first
					 && vPartsOrig[*ai.first] != vParts[v]) {
					
					add_edge(v, s->representatives[vPartsOrig[*ai.first]], *s->g);
					colors[s->partition[vParts[ 
					       s->representatives[vPartsOrig[*ai.first]]]]] = 1;
				}
				
			}
			
			bool found = false;
			for (i = 0; i < maxColor; i++) {
				if (colors[i] == 0) {
					if (DEBUG_LEVEL > 2) {
						cout<<"Found new suitable color for node "<<v<<"; the color is ";
						cout<<i<<endl;
					}
					
					found = true;
					s->partition[part] = i;
					break;
				}
			}
			
			/// No suitable color with color < maxColor found for replaced node
			if (!found) {
				if (DEBUG_LEVEL > 2) {
					cout<<"Found no new suitable color for node "<<v<<", choose random";
					cout<<" color"<<endl;
				}
				
				s->partition[vParts[v]] = rand() % maxColor;
			}
		}
				
		/// Rebuild all conflicts in vector "conflicts"
		conflicts.clear();
		for (vIter = vertices(*s->g); vIter.first != vIter.second; 
				  vIter.first++) {
			
			for (ai = adjacent_vertices(*vIter.first, *s->g); 
				  ai.first != ai.second; ai.first++) {
			
				if (s->partition[vParts[*ai.first]] ==
					 s->partition[vParts[*vIter.first]]) {
				
					if (DEBUG_LEVEL > 3) {
						cout<<"Conflicting node "<<*vIter.first<< " with color ";
						cout<<s->partition[vParts[*vIter.first]]<<" added to conflicts";
						cout<<endl;
					}
					conflicts.push_back(*vIter.first);
					break;
				}
			}
		}
	}
	
	/// Solution contains conlficts, revert changes
	if (conflicts.size() != 0) {
		if (DEBUG_LEVEL > 1) {
			cout<<"Conflict found"<<endl;
		}
		delete s;
		return new Solution(&best);
	}
	
	/// Solution is free of conflicts
	else {
		maxColor = 0;
		for (vIter = vertices(*s->g); vIter.first != vIter.second; 
			  vIter.first++) {
			
			if (s->partition[vParts[*vIter.first]] > maxColor) {
				maxColor = s->partition[vParts[*vIter.first]];
			}
		}
		s->colorsUsed = maxColor + 1;
		if (DEBUG_LEVEL > 1) {
			cout<<"changeNode uses "<<s->colorsUsed<<" colors"<<endl; 
		}
		Solution *temp = this->findLocalMin(*s, full);
		if (temp->colorsUsed < s->colorsUsed) {
			delete s;
			s = temp;
		}
	}
	return s;
}

Solution *changeNode::shuffleSolution(Solution& cur, Solution& full,
				 							  int numSteps) {
	return new Solution(&cur);
}
