#include "header/Solution.hpp"

using namespace std;
using namespace pcp;
using namespace boost;

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

Solution::~Solution() {
	delete g;
	delete[] partition;
	delete[] representatives;
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
	AdjIter i, end;

	for (tie(i, end) = adjacent_vertices(node, *this->g); i != end; i++)
		if (this->isPartitionColored(*i))
			colored++;

	return colored;
}

int Solution::minPossibleColor(Vertex node) {
	bool* colors = new bool[this->numParts];
	fill(colors, colors + this->numParts, false);
	AdjIter i, end;

	for (tie(i, end) = adjacent_vertices(node, *this->g); i != end; i++)
		if (this->isPartitionColored(*i))
			colors[this->partition[this->getPartition(*i)]] = true;

	for (int i = 0; i < this->numParts; i++) {
		if (!colors[i]) {
			delete[] colors;
			return i;
		}
	}

	return -1;
}

Solution Solution::fromPcpStream(istream& in) {
	Solution *s = new Solution();

	/// Convenient tokenizer to seperate the input string
	typedef boost::tokenizer<boost::char_separator<char> > Tok;
	boost::char_separator<char> sep; // default constructed
	
	/// Used to stores lines from the inputstream
	std::string buffer;

	if (DEBUG_LEVEL > 3) {
		cout<<"Reading from stdin"<<endl;
	}
	/// for easy access of the input parameters
	enum {
		vertices = 0,
		edges = 1,
		parts = 2
	};
	int nums[3]; // store first line numbers
	
	/// Read first line and store the parameters in nums array
	getline(in, buffer);
	Tok tok(buffer, sep);
	Tok::iterator iter = tok.begin(); 
	int i;
	for (i = 0; i < 3 && iter != tok.end(); i++)
		nums[i] = atoi((*iter++).c_str());
	
	if (DEBUG_LEVEL > 3) {
		cout<<"Read: vertices: "<<nums[vertices]<<" edges: "<<nums[edges];
		cout<<" partitions: "<<nums[parts]<<endl;
	}
	
	/// If the first line was malformed
	if (i != 3) {
		cerr<<"Wrong number of firstline arguments"<<endl;
		return NULL;
	}
	
	/// Initialize the solution to the read parameters
	s->partition = new int[nums[parts]];
	s->representatives = new int[nums[parts]];
	s->numParts = nums[parts];
	s->colorsUsed = nums[parts];

	/// Initialize the property maps for partition and vertexID
	VertexID_Map vertex_id = get(vertex_index2_t(), *s->g);
	VertexPart_Map vertex_part = get(vertex_index1_t(), *s->g);

	/// Read partition info and store it into the property map, do the same 
	/// for the "original" vertexID, so they can be compared on all graph
	for (i = 0; i < nums[vertices]; i++) {
		getline(in, buffer);
		Vertex v = add_vertex(*s->g);
		put(vertex_part, v, atoi(buffer.c_str())); 
		put(vertex_id, v, i);
		
		if (DEBUG_LEVEL > 3) {
			cout<<"Added vertex "<<i<<" to partition "<<atoi(buffer.c_str())<<endl;
		}
	}

	/// Read the input for edges between to vertices and add them to the 
	/// solution graph
	for (i = 0; i < nums[edges]; i++) {
		getline(in, buffer, ' ');
		int v1 = atoi(buffer.c_str());
		getline(in, buffer);
		int v2 = atoi(buffer.c_str());
	
		if (DEBUG_LEVEL > 3) {
			cout<<"Added edge ("<<v1<<"|"<<v2<<")"<<endl;
		}
		add_edge(v1, v2, *s->g);
	}
	if (DEBUG_LEVEL > 3) {
		cout<<"Reading input finished"<<endl;
	}
	return s;
}

Solution Solution::fromColStream(istream& in) {
	Solution *s = new Solution();

	char buffer[PARSER_COL_BUFFERSIZE];

	if (DEBUG_LEVEL > 3) {
		cout << "Reading from stdin" << endl;
	}
	
	/// Ignore all lines until first encounter of the p line
	while (in.get() != 'p')
		in.ignore(numeric_limits<streamsize>::max(), '\n');

	in.get();
	in.getline(buffer, PARSER_COL_BUFFERSIZE, ' ');

	if (strcmp(buffer, "edge") != 0) {
		cerr << "This parser is unable to read files using a FORMAT other than 'edge' ('" << buffer << "' specified)" << endl;
		return NULL;
	}

	in.getline(buffer, PARSER_COL_BUFFERSIZE, ' ');
	int vertices = atoi(buffer);
	in.getline(buffer, PARSER_COL_BUFFERSIZE);
	int edges = atoi(buffer);
	
	if (DEBUG_LEVEL > 3)
		cout<< "Reading " << edges << " edges ..." << endl;
	
	/// Initialize the property maps for partition and vertexID
	VertexID_Map vertex_id = get(vertex_index2_t(), *s->g);
	VertexPart_Map vertex_part = get(vertex_index1_t(), *s->g);
	
	int i;
	for (i = 0; i < vertices; i++) {
		Vertex v = add_vertex(*s->g);
		put(vertex_part, v, i); 
		put(vertex_id, v, i);
	}

	i = 0;
	while (i < edges / 2) {
		if (in.get() != 'e') {
			in.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		in.get();
		in.getline(buffer, PARSER_COL_BUFFERSIZE, ' ');
		int source = atoi(buffer);
		in.getline(buffer, PARSER_COL_BUFFERSIZE);
		int target = atoi(buffer);
		
		if (!(source < vertices && target << vertices)) {
			cerr << "Invalid edge (" << source << "|" << target << ") read." << endl;
			return NULL;
		}
		
		if (DEBUG_LEVEL > 3)
			cout << "Added edge (" << source << "|" << target << ")" << endl;

		add_edge(source, target, *s->g);
		i++;
	}

	if (DEBUG_LEVEL > 3)
		cout<<"Reading input finished"<<endl;

	s->partition = new int[vertices];
	s->representatives = new int[vertices];
	s->numParts = vertices;
	s->colorsUsed = vertices;

	return s;
}

void Solution::print(ostream& out) {
	vector<string> partitions(this->numParts);
	
	pair<VertexIter, VertexIter> vp;
	for (vp = vertices(*this->g); vp.first != vp.second; ++vp.first) {
		partitions[this->getPartition(*vp.first)] += "\t\t" + lexical_cast<string>(this->getOriginalId(*vp.first)) + ";\n";
 	}
 	
 	out << "graph pcp {" << endl;
 	out << "\tnode [shape = circle]" << endl;
 	out << "\tstyle = rounded" << endl;
 	
 	int counter = 0;
 	for (vector<string>::iterator i = partitions.begin(); i < partitions.end(); i++) {
 		out << "\tsubgraph cluster" << counter << " {" << endl;
 		out << *i << endl;
 		out << "\t\tcolor = " << graphvizColors[partition[counter]] << endl;
 		out << "\t\tlabel = " << counter++ << endl;
 		out << "\t}" << endl;
 	}
 	
 	graph_traits<Graph>::edge_iterator i, end;
	for (tie(i, end) = edges(*this->g); i != end; i++)
		out << "\t" << lexical_cast<string>(getOriginalId(source(*i, *this->g))) << " -- " << lexical_cast<string>(getOriginalId(target(*i, *this->g))) << ";" << endl;
 	
 	out << "}" << endl;
}
