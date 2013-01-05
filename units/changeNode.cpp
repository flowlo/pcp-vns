#include "../header/vns-priv.hpp"

using namespace std;
using namespace boost;
using namespace pcp;

const char *changeNode::name() {
	return "changeNode";
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
			vIndex[v] = rep;
			
			for (i = 0; i < s->numParts; i++) {
				colors[i] = 0;
			}
			
			/// Search matching edges for replacement vertex
			for (ai = adjacent_vertices(rep, *full.g); 
				  ai.first != ai.second; ai.first++) {
				  
				if (vIndex[s->representatives[vPartsOrig[*ai.first]]] == *ai.first
					 && vPartsOrig[*ai.first] != vParts[v]) {
					
					add_edge(v, s->representatives[vPartsOrig[*ai.first]], *s->g);
					colors[s->partition[vPartsOrig[*ai.first]]] = 1;
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
	Solution* ret = new Solution(&cur);
	
	VertexPart_Map vPartsOrig = get(vertex_index1_t(), *full.g);
	VertexID_Map vIndex = get(vertex_index2_t(), *ret->g);
	typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
	pair<AdjIter, AdjIter> ai;
	int colors[ret->numParts];
	int i = 0;
	
	for (int a = 0; a < numSteps; a++) {
		/// Select a random node in the original graph and replace it in the 
		/// solution graph
		Vertex replacement = rand() % num_vertices(*full.g);
		int part = vPartsOrig[replacement];
		Vertex v = ret->representatives[part];
		
		/// If it isn't already the current chosen node
		if (vIndex[ret->representatives[part]] != replacement) {
			
			clear_vertex(v, *ret->g);
			vIndex[v] = replacement;
			
			for (i = 0; i < ret->numParts; i++) {
				colors[i] = 0;
			}
			
			/// Search matching edges for replacement vertex
			for (ai = adjacent_vertices(replacement, *full.g); 
				  ai.first != ai.second; ai.first++) {
				  
				if (vIndex[ret->representatives[vPartsOrig[*ai.first]]] == *ai.first
					 && vPartsOrig[*ai.first] != part) {
					
					add_edge(v, ret->representatives[vPartsOrig[*ai.first]], *ret->g);
					colors[ret->partition[vPartsOrig[*ai.first]]] = 1;
				}
			}
			
			for (i = 0; i < ret->numParts; i++) {
				if (colors[i] == 0) {
					ret->partition[part] = i;
					break;
				}
			}
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
	
	ret->colorsUsed = maxColor + 1;
	return ret;
}
