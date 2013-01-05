#include "../header/vns-priv.hpp"

using namespace pcp;
using namespace std;
using namespace boost;

const char *changeColor::name() {
	return "changeColor";
}

const char changeColor::abbreviation() {
	return 'c';
}

Solution *changeColor::findLocalMin(Solution& curBest, Solution& full) {
	Solution *s = new Solution(&curBest);
	int maxColor = curBest.colorsUsed - 1;
	const int ITER_MAX = 100;
	pair<VertexIter, VertexIter> vIter;
	VertexPart_Map vParts = get(vertex_index1_t(), *s->g);

	if (DEBUG_LEVEL > 2) {
		cout<<"Starting changeColor"<<endl;
	}
	
	/// Change the color of all nodes with maxColor to a random color	
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
	std::vector<Vertex> conflicts(2);

	/// Add all conflicting nodes to vector "conflicts"
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
	
	/// Run until there are no more conflicts, or the iteration limit has been
	/// reached
	while (conflicts.size() != 0 && iter < ITER_MAX) {
		iter++;
		
		/// Choose random conflicting node for recoloring
		int random = rand() % conflicts.size();
		
		if (DEBUG_LEVEL > 3) {
			cout<<"Chose conflicting node "<<conflicts[random]<<endl;
		}
		
		for (i = 0; i < s->numParts; i++) {
			colors[i] = 0;
		}
		
		for (ai = adjacent_vertices(conflicts[random], *s->g); 
			  ai.first != ai.second; ai.first++) {
		
			colors[s->partition[vParts[*ai.first]]] = 1;
		}
		
		/// Search for new suitable color for Node
		bool found = false;
		for (i = 0; i < maxColor && !found; i++) {
			if (colors[i] == 0) {
				found = true;
			 	if (s->partition[conflicts[random]] != i) {
			 		if (DEBUG_LEVEL > 3) {
						cout<<"Found new suitable color "<<i<<" for Node ";
						cout<<conflicts[random]<<endl;
					}
					
					found = true;
					s->partition[vParts[conflicts[random]]] = i;
					break;
				}
			}
		}
		/// No suitable color found, try random recoloring of node
		if (!found) {
			int col = rand() % maxColor;
			s->partition[vParts[conflicts[random]]] = col;
			if (DEBUG_LEVEL > 3) {
				cout<<"Recolor node "<<conflicts[random]<<" with color ";
				cout<<col<<endl;
			}
		
			/// Search for conflicts with the randomly recolored node
			pair<AdjIter, AdjIter> aIter;
			for (aIter = adjacent_vertices(conflicts[random], *s->g); 
				  aIter.first != aIter.second; aIter.first++) {
				  
				for (i = 0; i < s->numParts; i++) {
					colors[i] = 0;
				}
			
				for (ai = adjacent_vertices(*aIter.first, *s->g); 
					  ai.first != ai.second; ai.first++) {
			
					colors[s->partition[vParts[*ai.first]]] = 1;
				}
			
				/// Try recoloring conflicting nodes
				bool found = false;
				for (i = 0; i < maxColor && !found; i++) {
					if (colors[i] == 0) {
						found = true;
					 	if (s->partition[vParts[*aIter.first]] != i) {
					 		if (DEBUG_LEVEL > 3) {
								cout<<"Found new suitable color "<<i<<" for Node ";
								cout<<*aIter.first<<endl;
							}
							found = true;
							s->partition[vParts[*aIter.first]] = i;
							break;
						}
					}
				}
				/// Conflict could not be resolved
				if (!found) {
					if (DEBUG_LEVEL > 2) {
						cout<<"Conflict could not be resolved for Node ";
						cout<<*aIter.first<<endl;
					}
				}
			}
		}
		
		/// Rebuild conflicting nodes
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
	
	/// New solution is not conflict free, return curBest
	if (conflicts.size() != 0) {
		if (DEBUG_LEVEL > 1) {
			cout<<"Conflict found"<<endl;
		}
		delete s;
		return new Solution(&curBest);
	}
	
	/// New solution is conflict free, count colorsUsed and return
	else {
		maxColor = 0;
		for (int i = 0; i < s->numParts; i++) {
			
			if (s->partition[i] > maxColor) {
				maxColor = s->partition[i];
			}
		}
		s->colorsUsed = maxColor + 1;
		Solution *temp = this->findLocalMin(*s, full);
		if (temp->colorsUsed < s->colorsUsed) {
			delete s;
			s = temp;
		}
	}
	if (DEBUG_LEVEL > 1) {
		cout<<"Change Color uses "<<s->colorsUsed<<" colors"<<endl; 
	}
	return s;
}

/// Reset all colors to unset, then proceed by selecting a node randomly,
/// recolor it with the least possible color. Proceed until all nodes are 
/// colored
Solution *changeColor::shuffleSolution(Solution& cur, Solution& full,
				 							  int numSteps) {
	
	Solution *ret = new Solution(&cur);
	VertexPart_Map vParts = get(vertex_index1_t(), *ret->g);
	std::vector<Vertex> uncolored;
	
	/// Reset all colors
	for (int i = 0; i < ret->numParts; i++) {
		ret->partition[i] = -1;
		uncolored.push_back(i);
	}
	
	std::random_shuffle(uncolored.begin(), uncolored.end());
	/// Proceed until all nodes are colored
	Vertex node = 0;
	int colors[ret->numParts];
	int maxColor = -1;
	while (uncolored.size() != 0) {
		node = uncolored.back();
		uncolored.pop_back();
		
		for (int i = 0; i < ret->numParts; i++) {
			colors[i] = 0;
		}
		
		
		typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
		pair<AdjIter, AdjIter> aIter;
		for (aIter = adjacent_vertices(node, *ret->g); aIter.first != aIter.second; 
			  aIter.first++) {
			  
			if (ret->partition[vParts[*aIter.first]] != -1) 
				colors[ret->partition[vParts[*aIter.first]]] = 1;
			
		}
		
		for (int i = 0; i < ret->numParts; i++) { 
			if (colors[i] == 0) {
				ret->partition[vParts[node]] = i;
				if (i > maxColor)
					maxColor = i;
					
				break;
			}
		}
	}
	ret->colorsUsed = maxColor + 1;
	
	return ret;
}
