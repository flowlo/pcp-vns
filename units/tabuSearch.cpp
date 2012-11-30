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
	
	for (vIter = vertices(*s->g); vIter.first != vIter.second; vIter.first++) {
		cout<<"Vertex "<<*vIter.first<<" is part "<<vParts[*vIter.first];
		cout<<" which is colored "<<s->partition[vParts[*vIter.first]]<<endl;
		if (s->partition[vParts[*vIter.first]] == maxColor) {
			int col = rand() % maxColor;
			cout<<"Recoloring "<<vIndex[*vIter.first]<<" with color ";
			cout<<col<<endl;
			s->partition[vParts[*vIter.first]] = rand() % maxColor;
		}
	}
	cout<<"Current conflicts in solution "<<conflicts(*s)<<endl;
	
	/*int conflicting[s->numParts];
	for (int i = 0; i < s->numParts; i++) {
		
	}*/
	
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
