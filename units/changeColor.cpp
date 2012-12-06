#include "../header/vns-priv.hpp"

using namespace pcp;
using namespace std;
using namespace boost;

const char *changeColor::name() {
	return "changeColor";
}

Solution *changeColor::findLocalMin(Solution& curBest, Solution& full) {
	Solution *s = new Solution(&curBest);
	int maxColor = curBest.colorsUsed - 1;
	const int ITER_MAX = 100;
	pair<VertexIter, VertexIter> vIter;
	VertexPart_Map vParts = get(vertex_index1_t(), *s->g);
	//VertexID_Map vIndex = get(vertex_index2_t(), *s->g);
	
	cout<<"Starting changeColor"<<endl;
	
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
	conflicts.clear();
	
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
	
	while (conflicts.size() != 0 && iter < ITER_MAX) {
		iter++;
		int random = rand() % conflicts.size();
		cout<<"Chose conflicting node "<<conflicts[random]<<endl;
		for (i = 0; i < s->numParts; i++) {
			colors[i] = 0;
		}
		
		for (ai = adjacent_vertices(conflicts[random], *s->g); 
			  ai.first != ai.second; ai.first++) {
		
			colors[s->partition[vParts[*ai.first]]] = 1;
		}
		
		bool found = false;
		for (i = 0; i < maxColor && !found; i++) {
			if (colors[i] == 0) {
				found = true;
			 	if (s->partition[conflicts[random]] != i) {
					cout<<"Found new suitable color "<<i<<" for Node ";
					cout<<conflicts[random]<<endl;
					found = true;
					s->partition[vParts[conflicts[random]]] = i;
					cout<<s->partition[vParts[conflicts[random]]]<<endl;
					break;
				}
			}
		}
		if (!found) {
			int col = rand() % maxColor;
			s->partition[vParts[conflicts[random]]] = col;
			cout<<"Recolor node "<<conflicts[random]<<" with color ";
			cout<<col<<endl;
		
			pair<AdjIter, AdjIter> aIter;
			for (aIter = adjacent_vertices(conflicts[random], *s->g); aIter.first != aIter.second; 
				  aIter.first++) {
				  
				for (i = 0; i < s->numParts; i++) {
					colors[i] = 0;
				}
			
				for (ai = adjacent_vertices(*aIter.first, *s->g); 
					  ai.first != ai.second; ai.first++) {
			
					colors[s->partition[vParts[*ai.first]]] = 1;
				}
			
				bool found = false;
				for (i = 0; i < maxColor && !found; i++) {
					if (colors[i] == 0) {
						found = true;
					 	if (s->partition[vParts[*aIter.first]] != i) {
							cout<<"Found new suitable color "<<i<<" for Node ";
							cout<<*aIter.first<<endl;
							found = true;
							s->partition[vParts[*aIter.first]] = i;
							break;
						}
					}
				}
				if (!found) {
					cout<<"Conflict could not be resolved for Node ";
					cout<<*aIter.first<<endl;
					conflicts.push_back(*aIter.first);
				}
			}
		}
		conflicts.clear();
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
	}
	
	if (conflicts.size() != 0) {
		cout<<"Conflict found"<<endl;
		delete s;
		return new Solution(&curBest);
	}
	else {
		maxColor = 0;
		for (vIter = vertices(*s->g); vIter.first != vIter.second; 
			  vIter.first++) {
			
			if (s->partition[vParts[*vIter.first]] > maxColor) {
				maxColor = s->partition[vParts[*vIter.first]];
			}
		}
		s->colorsUsed = maxColor + 1;
		Solution *temp = this->findLocalMin(*s, full);
		if (temp->colorsUsed < s->colorsUsed) {
			delete s;
			s = temp;
		}
	}
	cout<<"Change Color uses "<<s->colorsUsed<<" colors"<<endl; 
	return s;
}

Solution *changeColor::shuffleSolution(Solution& cur, Solution& full,
				 							  int numSteps) {
	return new Solution(&cur);
}
