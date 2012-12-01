#include "../header/vns-priv.hpp"

using namespace pcp;
using namespace std;
using namespace boost;

const char* tabuSearch::name() {
	return "Tabu Search";
}

Solution* tabuSearch::findLocalMin(Solution& curBest, Solution& full) {
	cout<<"Hello this is tabusearch speaking"<<endl;
	cout<<"Current conflicts in curBest "<<conflicts(curBest)<<endl;
	cout<<"curBest has maxColor "<<curBest.colorsUsed<<endl;
	
	
	Solution* s = new Solution(&curBest);
	int maxColor = curBest.colorsUsed - 1;
	pair<VertexIter, VertexIter> vIter;
	VertexPart_Map vParts = get(vertex_index1_t(), *s->g);
	VertexID_Map vIndex = get(vertex_index2_t(), *s->g);
	int iterations = 0;
	
	
	for (vIter = vertices(*s->g); vIter.first != vIter.second; vIter.first++) {
		//cout<<"Vertex "<<*vIter.first<<" is part "<<vParts[*vIter.first];
		//cout<<" which is colored "<<s->partition[vParts[*vIter.first]]<<endl;
		if (s->partition[vParts[*vIter.first]] == maxColor) {
			int col = rand() % maxColor;
			cout<<"Recoloring "<<vIndex[*vIter.first]<<" with color ";
			cout<<col<<endl;
			s->partition[vParts[*vIter.first]] = rand() % maxColor;
		}
	}
	cout<<"Current conflicts in solution "<<conflicts(*s)<<endl;
	
	typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;
	pair<AdjIter, AdjIter> ai;
	int c = 0;
	typedef std::tuple<bool, int, int> tabuInfo; 
	list<tabuInfo> tabuList;
	for (int i = 0; i < s->numParts; i++) {
		for (ai = adjacent_vertices(i, *s->g); ai.first != ai.second; 
			 ai.first++) {
		 	if (s->partition[vParts[i]] == s->partition[vParts[*ai.first]]) {
		 		cout<<"Partition "<<vParts[i]<<" is in a conflict"<<endl;
		 		tabuInfo temp(true, vParts[i], numeric_limits<int>::max());
		 		tabuList.push_front(temp);
		 		c++;
		 		break;
		 	}
		}
	}
	
	list<tabuInfo>::iterator li;
	li = tabuList.begin();
	int i = 0;
	while (!tabuList.empty()) {	
		int select = rand() % tabuList.size();
		for (i = 0; i < select; i++) {
			li++;
		}
		int part = get<1>(*li);
		tabuList.erase(li);
		pair<VertexIter, VertexIter> vi;
		list<Vertex> nodes;
		list<Vertex>::iterator nodeIter;
		VertexPart_Map parts = get(vertex_index1_t(), *full.g);
		for (vi = vertices(*full.g); vi.first != vi.second; vi.first++) {
			if (parts[*vi.first] == part) {
				nodes.push_front(*vi.first);
			}
		}
		
		bool reduction = false;
		int maxConflicts = numeric_limits<int>::max();
		for(nodeIter = nodes.begin(); nodeIter != nodes.end() && !reduction; 
			nodeIter++) {
			for (i = 0; i < maxColor && !reduction; i++) {
				bool found = false;
				list<tabuInfo>::iterator li2;
				for (li2 = tabuList.begin(); li2 != tabuList.end() && !found; 
					li2++) {
					if (get<2>(*li2) > iter) {
						tabuList.erase(li2);
					}
					else if ((get<0>(*li2) && get<1>(*li2) == parts[*nodeIter]) ||
						(!get<0>(*li2) && get<1>(*li2) == *nodeIter)) {
						// TODO: recolor
					}
				}
			}
		}
	}
	if (conflicts(s) == 0) {
		
	}
	return s;			
}

int tabuSearch::conflicts(Solution& s) {	
	typedef graph_traits<Graph>::edge_iterator EdgeIterator;
	pair<EdgeIterator, EdgeIterator> ei;
	int conflicts = 0;
	
	VertexPart_Map parts = get(vertex_index1_t(), *s.g);
	
	for (ei = edges(*s.g); ei.first != ei.second; ei.first++) {
		if (s.partition[parts[source(*ei.first, *s.g)]] == 
			s.partition[parts[target(*ei.first, *s.g)]]) {
			conflicts++;
		}
	}
	return conflicts;
}

Solution* tabuSearch::shuffleSolution(Solution& cur, Solution& full, int numSteps) {
	return &full;
}
