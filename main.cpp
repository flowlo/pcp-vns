#include <iostream>                  // for //cout
#include <string>
#include <utility>                   // for pair
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/tokenizer.hpp>
#include <boost/graph/graphviz.hpp>
#include "pcp.hpp"
#include "parser.hpp"
#include "oneStepCd.hpp"
#include "vns.hpp"

using namespace std;
using namespace pcp;
using namespace boost;


Solution::Solution() {
	this->g = new Graph(0);
	this->colorsUsed = 0;
}

Solution::Solution(Solution *toCopy) {
	this->g = new Graph(0);
	this->numParts = toCopy->numParts;
	this->partition = new int[numParts];
	this->colorsUsed = toCopy->colorsUsed;
	copy_graph(*toCopy->g, *this->g);
}


int main(int, char*[]) {
	
	// declare a graph object
	Graph g(0);
	Solution fullG = new Solution;
	
	if (!readGraph(cin, fullG)) {
		cerr<<"Error reading from stdin"<<endl;
	}
	
	Solution onestep = onestepCD(fullG);
	
	graph_traits<Graph>::edge_iterator ei, ei_end;
	

	VertexID_Map vertex_id = get(vertex_index2_t(), *onestep.g);
	VertexPart_Map vertex_part = get(vertex_index1_t(), *onestep.g);

	cout<<"Onestep solution uses "<<onestep.colorsUsed<<" colors"<<endl;
	
	Solution best = vnsRun(onestep, fullG, 1, 0);
	
	
	write_graphviz(cout, *best.g, make_label_writer(vertex_id));
	
	return 0;
}
