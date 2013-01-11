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
	this->partNodes = NULL;
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

Solution* Solution::fromPcpStream(istream& in) {
	Solution *s = new Solution();
	
	int vertices, edges, partitions;
	cin >> vertices >> edges >> partitions;
	
	if (DEBUG_LEVEL > 3) {
		cout << "Reading " << vertices << " vertices, " << edges << " edges and ";
		cout << partitions << " partitons ..." << endl;
	}
	
	/// Initialize the solution to the read parameters
	s->partition = new int[partitions];
	s->representatives = new int[partitions];
	s->partNodes = new vector<Vertex>[partitions];
	s->numParts = partitions;
	s->colorsUsed = partitions;

	/// Initialize the property maps for partition and vertexID
	VertexID_Map vertex_id = get(vertex_index2_t(), *s->g);
	VertexPart_Map vertex_part = get(vertex_index1_t(), *s->g);

	/// Read partition info and store it into the property map, do the same 
	/// for the "original" vertexID, so they can be compared on all graph
	int i, part;
	for (i = 0; i < vertices; i++) {
		cin >> part;
		Vertex v = add_vertex(*s->g);
		put(vertex_part, v, part);
		put(vertex_id, v, i);
		s->partNodes[part].push_back(v);
		
		if (DEBUG_LEVEL > 3)
			cout << "Added vertex " << i << " to partition " << part << "." << endl;
	}

	/// Read the input for edges between to vertices and add them to the 
	/// solution graph
	for (i = 0; i < edges; i++) {
		int source, target;
		cin >> source >> target;

		if (DEBUG_LEVEL > 3) {
			cout << "Added edge (" << source << "|" << target << ")." << endl;
		}
		add_edge(source, target, *s->g);
	}
	
	if (DEBUG_LEVEL > 3)
		cout << "Parsing input finished successfully!" << endl;

	return s;
}

Solution* Solution::fromColStream(istream& in) {
	Solution *s = new Solution;

	char buffer[PARSE_BUFFERSIZE];

	/// Ignore all lines until first encounter of the p line
	while (in.get() != 'p')
		in.ignore(numeric_limits<streamsize>::max(), '\n');

	in.get();
	in.getline(buffer, PARSE_BUFFERSIZE, ' ');

	if (strcmp(buffer, "edge") != 0) {
		cerr << "This parser is unable to read files using a FORMAT other than 'edge' ('" << buffer << "' specified)" << endl;
		return NULL;
	}

	int vertices, edges;
	cin >> vertices >> edges;
	
	if (DEBUG_LEVEL > 3)
		cout << "Reading " << edges << " edges ..." << endl;
		
	if (in.get() != '\n') {
		cerr << "Malformed p-line detected (not terminated by newline)!" << endl;
		return NULL;
	}
	
	/// Initialize the property maps for partition and vertexID
	VertexID_Map vertex_id = get(vertex_index2_t(), *s->g);
	VertexPart_Map vertex_part = get(vertex_index1_t(), *s->g);
	
	s->partNodes = new vector<Vertex>[vertices];
	
	int i;
	for (i = 0; i < vertices; i++) {
		Vertex v = add_vertex(*s->g);
		put(vertex_part, v, i); 
		put(vertex_id, v + 1, i);
		s->partNodes[i].push_back(v);
	}

	i = 0;
	while (i++ < edges) {
		if (in.get() != 'e') {
			cerr << "This parser is not capable of handling other information than edge definitions after the p-line." << endl;
			return NULL;
		}

		int source, target;
		cin >> source >> target;
		
		if (!(source <= vertices && target <= vertices)) {
			cerr << "Invalid edge (" << source << "|" << target << ") read." << endl;
			return NULL;
		}
		
		if (DEBUG_LEVEL > 3)
			cout << "Added edge (" << source << "|" << target << ")." << endl;

		add_edge(source - 1, target - 1, *s->g);

		if (in.get() != '\n') {
			cerr << "Malformed edge definition read (edge line " << (i + 1) << " not terminated by newline)!" << endl;
			return NULL;
		}
	}

	if (DEBUG_LEVEL > 3)
		cout << "Reading input successfully finished!" << endl;

	s->partition = new int[vertices];
	s->representatives = new int[vertices];
	s->numParts = vertices;
	s->colorsUsed = vertices;

	return s;
}

Solution* Solution::fromColBStream(istream& in) {
	Solution *s = new Solution;
	char buffer[PARSE_BUFFERSIZE];
	
	int length;
	cin >> length;
	
	while (cin.get() != 'p')
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cin.get();
	cin.getline(buffer, PARSE_BUFFERSIZE, ' ');

	if (strcmp(buffer, "edge") != 0) {
		cerr << "This parser is unable to read files using a FORMAT other than 'edge' ('" << buffer << "' specified)" << endl;
		return NULL;
	}

	int vertices, edges, i, j;
	cin >> vertices >> edges;
	char bitmap[vertices][vertices / 8];
	
	VertexID_Map vertex_id = get(vertex_index2_t(), *s->g);
	VertexPart_Map vertex_part = get(vertex_index1_t(), *s->g);
	
	s->partNodes = new vector<Vertex>[vertices];
	
	for (i = 0; i < vertices; i++) {
		Vertex v = add_vertex(*s->g);
		put(vertex_part, v, i); 
		put(vertex_id, v + 1, i);
		s->partNodes[i].push_back(v);

		cin.read(bitmap[i], (i + 8) / 8);
		cout << bitmap[i];
	}

	for (i = 0; i < vertices; i++)
		for (j = 0; j <= i; j++)
				if ((bitmap[i][j >> 3] & (1 << (7 - (j & 7)))) != 0) {
					cout << "Added edge (" << i << "|" << j << ")." << endl;
					add_edge(i, j, *s->g);
				}
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
