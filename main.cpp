#include "header/main.hpp"

using namespace std;
using namespace pcp;
using namespace boost;
using namespace boost::program_options;

Solution::Solution() {
	this->g = new Graph(0);
	this->colorsUsed = 0;
	this->partitionMap = get(vertex_index1_t(), *this->g);
	this->idMap = get(vertex_index2_t(), *this->g);
}

Solution::Solution(Solution *toCopy) {
	this->g = new Graph(0);
	this->numParts = toCopy->numParts;
	this->partition = new int[this->numParts];
	this->representatives = new int[this->numParts];

	for (int i = 0; i < this->numParts; i++) {
		this->partition[i] = toCopy->partition[i];
		this->representatives[i] = toCopy->representatives[i];
	}
	this->colorsUsed = toCopy->colorsUsed;
	copy_graph(*toCopy->g, *this->g);
	this->partitionMap = get(vertex_index1_t(), *this->g);
	this->idMap = get(vertex_index2_t(), *this->g);
}

int Solution::getPartition(Vertex v) {
	return get(this->partitionMap, v);
}

int Solution::getOriginalId(Vertex v) {
	return get(this->idMap, v);
}

int Solution::getPartitionColor(Vertex v) {
	return partition[getPartition(v)];
}

void Solution::setPartitionColor(Vertex v, int color) {
	partition[getPartition(v)] = color;
}

bool Solution::isPartitionColored(Vertex v) {
	return getPartitionColor(v) != -1;
}

int Solution::getColorDegree(Vertex node) {
	int colored = 0;
	typename graph_traits<Graph>::adjacency_iterator i, end;

	for (tie(i, end) = adjacent_vertices(node, *this->g); i != end; i++)
		if (this->isPartitionColored(*i))
			colored++;

	return colored;
}

int Solution::minPossibleColor(Vertex node) {
	bool colors[this->numParts];
	fill(colors, colors + this->numParts, false);
	typename graph_traits<Graph>::adjacency_iterator i, end;

	for (tie(i, end) = adjacent_vertices(node, *this->g); i != end; i++)
		if (this->isPartitionColored(*i))
			colors[this->partition[this->getPartition(*i)]] = true;

	for (int i = 0; i < this->numParts; i++)
		if (!colors[i])
			return i;

	return -1;
}

int DEBUG_LEVEL = 2;

int main(int argc, char* argv[]) {
	string units, printFile;
	int unsuccessfulShake, shakeStart, shakeIncrement, maxTime;
	
	options_description options("General options");
	options.add_options()
		("help,h", "produce help message")
		("print,p", value<string>(&printFile), "set print file (*.gv)")
		("debug,d", value<int>(&DEBUG_LEVEL)->default_value(0), "set debug level")
	;
	
	options_description vnsOptions("Variable Neighborhood Search options");
	vnsOptions.add_options()
		("units,n", value<string>(&units)->default_value("nc"), "set units")
		("shakeStart,s", value<int>(&shakeStart)->default_value(0), "set shake start")
		("shakeIncrement,i", value<int>(&shakeIncrement)->default_value(10), "set shake increment")
		("unsuccessfulShake, u", value<int>(&unsuccessfulShake)->default_value(10), "set unsuccessful shake threshold")
		("maxTime,t", value<int>(&maxTime)->default_value(10), "set VNS running time (seconds)")
		("checkFinal,c", "disable final check after VNS has finished")
		("checkIntermediate,m", "enable check after each improvement/shake")
	;
	
	options_description all("Allowed options");
	all.add(options).add(vnsOptions);
	
	variables_map vm;
	store(parse_command_line(argc, argv, all), vm);
	notify(vm);
	
	if (vm.count("help")) {
		cout << all << endl;
		return 0;
	}
	
	if (DEBUG_LEVEL > 4) {
		DEBUG_LEVEL = 4;
		
		cout << "The specified debug level (-d) exceeds"
		<< " the maximum. It was set to " << DEBUG_LEVEL
		<< "." << endl;
	}
	else if (DEBUG_LEVEL < 0) {
		DEBUG_LEVEL = 0;
	}
	
	Solution *fullG = new Solution();
	
	if (!readGraph(cin, *fullG)) {
		cerr << "Error reading from stdin!" << endl;
	}
	
	Solution *onestep = onestepCD(*fullG);
	
	if (DEBUG_LEVEL > 2)
		cout << "Onestep solution uses " << onestep->colorsUsed << " colors." << endl;
	
	Solution *best = vnsRun(*onestep, *fullG, units, unsuccessfulShake, shakeStart, shakeIncrement, maxTime, vm.count("checkIntermediate"), vm.count("checkFinal"));
	
	if (best == NULL)
		return -1;
	
	if (vm.count("print")) {
		ofstream out(vm["print"].as<string>());
		
		if (out.fail()) {
			cerr << "Error when trying to access file: " << vm["print"].as<string>() << endl;
			return -1;
		}
		
		VertexID_Map vertex_id = get(vertex_index2_t(), *best->g);
		write_graphviz(out, *best->g, make_label_writer(vertex_id));
		
		out.flush();
		out.close();
		
		if (DEBUG_LEVEL > 1)
			cout << "Printing to '" << vm["print"].as<string>() << "' done!" << endl;
	}

	return 0;
}
