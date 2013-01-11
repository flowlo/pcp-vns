#include "../header/vns-priv.hpp"

using namespace pcp;
using namespace std;
using namespace boost;

const string changeColor::getName() {
	return "changeColor";
}

const char changeColor::getAbbreviation() {
	return getStaticAbbreviation();
}

const char changeColor::getStaticAbbreviation() {
	return 'c' ;
}

Solution *changeColor::findLocalMin(Solution& curBest, Solution& full) {
	Solution *s = &curBest;
	int maxColor = curBest.colorsUsed - 1;
	const int ITER_MAX = s->numParts * 20;
	pair<VertexIter, VertexIter> vIter;
	VertexPart_Map vParts = get(vertex_index1_t(), *s->g);

	if (DEBUG_LEVEL > 2) {
		cout<<"Starting changeColor"<<endl;
	}
	
	/// Change the color of all nodes with maxColor to a random color	
	for (vIter = vertices(*s->g); vIter.first != vIter.second; vIter.first++) {
		if (s->getPartitionColor(*vIter.first) == maxColor) {
			int col = rand() % maxColor;
			s->setPartitionColor(*vIter.first, col);
			if (DEBUG_LEVEL > 3) {	
				cout<<"Recoloring "<<*vIter.first<<" with color ";
				cout<<col<<endl;
			}
		}
	}

	pair<AdjIter, AdjIter> ai;
	int colors[s->numParts];
	int i = 0;
	int iter = 0;
	std::vector<Vertex> conflicts;

	/// Add all conflicting nodes to vector "conflicts"
	for (vIter = vertices(*s->g); vIter.first != vIter.second; 
			  vIter.first++) {
			
		for (ai = adjacent_vertices(*vIter.first, *s->g); 
			  ai.first != ai.second; ai.first++) {
			
			if (s->getPartitionColor(*ai.first) ==
				 s->getPartitionColor(*vIter.first)) {
				
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
		
			colors[s->getPartitionColor(*ai.first)] = 1;
		}
		
		/// Search for new suitable color for Node
		bool found = false;
		for (i = 0; i < maxColor && !found; i++) {
			if (colors[i] == 0) {
				if (DEBUG_LEVEL > 3) {
					cout<<"Found new suitable color "<<i<<" for Node ";
					cout<<conflicts[random]<<endl;
				}
				
				found = true;
				s->setPartitionColor(conflicts[random], i);
				break;
			}
		}
		/// No suitable color found, try random recoloring of node
		if (!found) {
			int col = rand() % maxColor;
			s->setPartitionColor(conflicts[random], col);
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
			
					colors[s->getPartitionColor(*ai.first)] = 1;
				}
			
				/// Try recoloring conflicting nodes
				bool found = false;
				for (i = 0; i < maxColor && !found; i++) {
					if (colors[i] == 0) {
						if (DEBUG_LEVEL > 3) {
							cout<<"Found new suitable color "<<i<<" for Node ";
							cout<<*aIter.first<<endl;
						}
						found = true;
						s->setPartitionColor(*aIter.first, i);
						break;
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
			
				if (s->getPartitionColor(*ai.first) ==
					 s->getPartitionColor(*vIter.first)) {
					
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
		s->colorsUsed = s->numParts;
		return s;
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
		
		if (DEBUG_LEVEL > 1) {
			cout<<"Change Color uses "<<s->colorsUsed<<" colors"<<endl; 
		}
		
		Solution *temp = new Solution(s);
		temp = this->findLocalMin(*temp, full);
		if (temp->colorsUsed < s->colorsUsed) {
			delete s;
			s = temp;
		}
		else 
			delete temp;
	}
	return s;
}

/// Reset all colors to unset, then proceed by selecting a node randomly,
/// recolor it with the least possible color. Proceed until all nodes are 
/// colored
Solution *changeColor::shuffleSolution(Solution& cur, Solution& full,
				 							  int numSteps) {
	if (DEBUG_LEVEL > 3) {
		cout<<"Shaking with changeColor with"<<endl;
	}
	
	Solution *ret = &cur;
	vector<Vertex> uncolored;
	
	/// Reset all colors
	for (int i = 0; i < numSteps; i++) {
		uncolored.push_back(i % ret->numParts);
	}
	
	random_shuffle(uncolored.begin(), uncolored.end());
	/// Proceed until all nodes are colored
	Vertex node = 0;
	int color;
	int maxColor = ret->colorsUsed - 1;
	while (uncolored.size() != 0) {
		node = uncolored.back();
		uncolored.pop_back();
		color = ret->minPossibleColor(node);
		ret->setPartitionColor(node, color);
		if (color > maxColor)
			maxColor = color;
	}
	ret->colorsUsed = maxColor + 1;
	
	return ret;
}
