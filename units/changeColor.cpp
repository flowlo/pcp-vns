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
	VertexIter i, iEnd;
	AdjIter a, aEnd;
	const int colorRetries = maxColor;

	if (DEBUG_LEVEL > 2) {
		cout<<"Starting changeColor"<<endl;
	}
	
	for (tie(i, iEnd) = vertices(*s->g); i != iEnd; i++) {
		if (s->getPartitionColor(*i) == maxColor) {
			if (DEBUG_LEVEL > 3) {
				cout<<"vertex "<<*i<<"has maxColor; ";
			}
			
			int j;
			for (j = 0; j < colorRetries; j++) {
				int nCol = rand() % maxColor - 1;
				s->setPartitionColor(*i, nCol);
			
				if (DEBUG_LEVEL > 3) {
					cout<<"new color is "<<nCol<<endl;
				}
			
				bool allColored = true;
				for (tie(a, aEnd) = adjacent_vertices(*i, *s->g); a != aEnd; a++) {
					if (s->getPartitionColor(*a) == nCol) {
						int recolor = s->minPossibleColor(*a);
						if (recolor >= maxColor) {
							allColored = false;
							break;
						}
						else {
							s->setPartitionColor(*a, recolor);
						}
					}
				}
				if (allColored)
					break;
			}
			if (j == colorRetries) {
				if (DEBUG_LEVEL > 1) {
					cout<<"Conflict found: could not resolve conflict with node ";
					cout<<*i<<endl;
				}
				s->colorsUsed = s->numParts;
				return s;
			}
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
		cout<<"Change Color uses "<<s->colorsUsed<<" colors"<<endl; 
	}
	
	Solution *temp = new Solution(s);
	temp = this->findLocalMin(*temp, full);
	if (temp->colorsUsed < s->colorsUsed) {
		//delete s;
		s = temp;
	}
	//else 
		//delete temp;
		
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
		uncolored.push_back(rand() % ret->numParts);
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
