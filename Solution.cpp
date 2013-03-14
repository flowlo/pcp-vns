#include "include/Solution.hpp"

using namespace std;
using namespace pcp;
using namespace boost;
using namespace boost::assign;

// Create a new (empty solution)
Solution::Solution() {
	this->g = new Graph(0);
	this->colorsUsed = 0;
	this->partitionMap = get(vertex_index1_t(), *this->g);
	this->idMap = get(vertex_index2_t(), *this->g);
	// Fun with pointers!
	this->copyCounter = new int;
	*this->copyCounter = 1;
}

// Copy an existing solution, this will use the same graph object as the 
// original, to cut these ties, use requestDeepCopy()
Solution::Solution(Solution *toCopy) {
	this->partNodes = NULL;
	this->g = toCopy->g;
	this->copyCounter = toCopy->copyCounter;
	// increment copycounter, to keep track of referencing solution
	*this->copyCounter += 1;
	this->numParts = toCopy->numParts;
	this->partition = new int[this->numParts];
	this->representatives = toCopy->representatives;
	this->colorsUsed = toCopy->colorsUsed;
	this->partitionMap = get(vertex_index1_t(), *this->g);
	this->idMap = get(vertex_index2_t(), *this->g);
	
	for (int i = 0; i < this->numParts; i++) {
		this->partition[i] = toCopy->partition[i];
	}
}

// Destructor
Solution::~Solution() {
	// Decrement copyCounter, if <= 0 delete all information about the referenced
	// graph and corresponding arrays
	*copyCounter -= 1;
	if (*copyCounter <= 0) {
		delete g;
		delete copyCounter;
		delete[] representatives;
	}
	delete[] partition;
	delete[] partNodes;
}

// Return the PartitionId of a Vertex v
int Solution::getPartition(Vertex v) {
	return get(this->partitionMap, v);
}

// Return the OriginalId of a Vertex v
unsigned int Solution::getOriginalId(Vertex v) {
	return get(this->idMap, v);
}

// Set the OriginalId of Vertex v to id 
void Solution::setOriginalId(Vertex v, int id) {
	put(this->idMap, v, id);
	
	// Update ubigraph if necessary
	#ifdef ubigraph
	ubigraph_set_vertex_attribute(getOriginalId(v), "label", (to_string(getOriginalId(v)) + " [" + to_string(getPartition(v)) + "]").c_str());
	#endif
}

// Get the color of a specific Vertex v, or rather of the corresponding partition
// of the Vertex
int Solution::getPartitionColor(Vertex v) {
	return partition[getPartition(v)];
}

// Set the color of Vertex v, and update colorsUsed if appropriate
void Solution::setPartitionColor(Vertex v, int color) {
	partition[getPartition(v)] = color;

	if (color >= colorsUsed)
		colorsUsed = color + 1;

	// Update ubigraph if necessary
	#ifdef ubigraph
	ubigraph_set_vertex_attribute(getOriginalId(v), "color", hexColors[color % hexColors.size()].c_str());
	usleep(500);
	#endif
}

// Returns true if a Vertex has already been colored
bool Solution::isPartitionColored(Vertex v) {
	return getPartitionColor(v) != -1;
}

// Add a Vertex with partitionID part und id to the solution
void Solution::addVertex(int part, Vertex id) {
	Vertex v = add_vertex(*this->g);
	partitionMap[v] = part;
	idMap[v] = id;
	partNodes[part].push_back(v);
	
	// update ubigraph if necessary
	#ifdef ubigraph
	ubigraph_new_vertex_w_id(getOriginalId(v));
	ubigraph_set_vertex_attribute(getOriginalId(v), "label", (to_string(getOriginalId(v)) + " [" + to_string(getPartition(v)) + "]").c_str());
	
	if (part < 9 && this->numParts < 9)
		ubigraph_change_vertex_style(getOriginalId(v), part);
	#endif
}

// Remove a Vertex id from the graph
void Solution::removeVertex(Vertex id) {
	#ifdef ubigraph
	ubigraph_remove_vertex(getOriginalId(id));
	usleep(500000);
	#endif

	clear_vertex(id, *g);
	remove_vertex(id, *g);
}

// Add an edge between v1 and v2 to the graph
void Solution::addEdge(Vertex v1, Vertex v2) {
	add_edge(v1, v2, *g);
	
	// update ubigraph
	#ifdef ubigraph
	v1 = getOriginalId(v1);
	v2 = getOriginalId(v2);
	if (v1 > v2) {
		Vertex temp = v1;
		v1 = v2;
		v2 = temp;
	}
	ubigraph_new_edge_w_id(((v1 << 16) | v2), v1, v2);
	usleep(100000);
	#endif
}

// Replace an existing vertex toR with Vertex rep in the original graph
// toR, the Vertex to be replaced
// rep, the Vertex toR will be replaced with
// full, the ful Solution graph
void Solution::replaceVertex(Vertex toR, Vertex rep, Solution& full) {
	// Clear the vertex toR of all edges
	clear_vertex(toR, *g);
	
	// update ubigraph
	#ifdef ubigraph
	ubigraph_remove_vertex(getOriginalId(toR));
	ubigraph_new_vertex_w_id(rep);
	ubigraph_set_vertex_attribute(rep, "label", (to_string(getOriginalId(rep)) + " [" + to_string(getPartition(rep)) + "]").c_str());
	ubigraph_set_vertex_attribute(rep, "color", hexColors[getPartitionColor(toR) % hexColors.size()].c_str());
	
	int part = full.getPartition(rep);
	if (part < 9)
		ubigraph_change_vertex_style(rep, part);
	usleep(50000);
	#endif
	
	// Set the originalId of toR to rep
	setOriginalId(toR, rep);
	
	// Check which edges should be added
	AdjIter a, aEnd;
	for (tie(a, aEnd) = adjacent_vertices(rep, *full.g); a != aEnd;
		  a++) {
		
		if (full.getPartition(*a) != getPartition(toR) && *a ==
			 getOriginalId(representatives[full.getPartition(*a)])) {
			
			// add edge
			add_edge(toR, representatives[full.getPartition(*a)], *g);
			
			// update ubigraph
			#ifdef ubigraph
			Vertex v1, v2;
			v1 = full.getOriginalId(rep);
			v2 = full.getOriginalId(*a);
			if (v1 > v2) {
				Vertex temp = v1;
				v1 = v2;
				v2 = temp;
			}
			ubigraph_new_edge_w_id(((v1 << 16) | v2), v1, v2);
			usleep(80000);
			#endif
		}
	}
}

// Redraw method only for ubigraph
#ifdef ubigraph
void Solution::redraw() {
	// Set prerequisites
	prepareUbigraph();

	// Add all vertices
	VertexIter v, vEnd;
	for (tie(v, vEnd) = vertices(*g); v != vEnd; v++) {
		ubigraph_new_vertex_w_id(getOriginalId(*v));
		ubigraph_set_vertex_attribute(getOriginalId(*v), "label", (to_string(getOriginalId(*v)) + " [" + to_string(getPartition(*v)) + "]").c_str());
		ubigraph_set_vertex_attribute(getOriginalId(*v), "color", hexColors[getPartitionColor(*v) % hexColors.size()].c_str());
	}

	// Add all edges
	EdgeIter e, eEnd;
	for(tie(e, eEnd) = edges(*g); e != eEnd; e++) {
		Vertex v1, v2;
		v1 = getOriginalId(source(*e, *g));
		v2 = getOriginalId(target(*e, *g));
		if (v1 > v2) {
			Vertex temp = v1;
			v1 = v2;
			v2 = temp;
		}
		ubigraph_new_edge_w_id(((v1 << 16) | v2), v1, v2);
	}
	usleep(500000);
}
#endif

// Redraw with a shift of the id, as to not colide with existing one
#ifdef ubigraph
void Solution::redraw(int shift) {
	VertexIter v, vEnd;
	for (tie(v, vEnd) = vertices(*g); v != vEnd; v++) {
		ubigraph_new_vertex_w_id(getOriginalId(*v) + shift);
		ubigraph_set_vertex_attribute(getOriginalId(*v) + shift, "label", (to_string(getOriginalId(*v)) + " [" + to_string(getPartition(*v)) + "]").c_str());
		ubigraph_set_vertex_attribute(getOriginalId(*v) + shift, "color", hexColors[getPartitionColor(*v) % hexColors.size()].c_str());
	}
	
	EdgeIter e, eEnd;
	for(tie(e, eEnd) = edges(*g); e != eEnd; e++) {
		Vertex v1, v2;
		v1 = getOriginalId(source(*e, *g));
		v2 = getOriginalId(target(*e, *g));
		if (v1 > v2) {
			Vertex temp = v1;
			v1 = v2;
			v2 = temp;
		}
		ubigraph_new_edge_w_id(((v1 << 16) | v2) + shift, v1 + shift, v2 + shift);
	}
	usleep(500000);
}
#endif

// Cut ties between a copy and the underlying graph by making a copy of the graph
void Solution::requestDeepCopy() {
	// Copy graph
	Graph *cp = g;
	g = new Graph(*g);
	// Reset property maps
	this->partitionMap = get(vertex_index1_t(), *g);
	this->idMap = get(vertex_index2_t(), *g);

	// Copy representatives array
	int *rep = representatives;
	representatives = new int[numParts];
	for (int i = 0; i < numParts; i++) {
		representatives[i] = rep[i];
	}

	// delete the old copy if counter reached zero
	*copyCounter -= 1;
	if (*copyCounter <= 0) {
		delete copyCounter;
		delete cp;
		delete[] rep;
	}
	copyCounter = new int;
	*copyCounter = 1;
}

// Get the colorDegree (i.e. the number of colored neighbors) of Vertex node
int Solution::getColorDegree(Vertex node) {
	int colored = 0;
	AdjIter i, end;

	// check all adjacent vertices for color
	for (tie(i, end) = adjacent_vertices(node, *this->g); i != end; i++)
		if (this->isPartitionColored(*i))
			colored++;

	return colored;
}

// Returns the minimal possible color for a Vertex node
int Solution::minPossibleColor(Vertex node) {
	bool* colors = new bool[this->numParts];
	fill(colors, colors + this->numParts, false);
	AdjIter i, end;
	
	// Mark all colors which are used by neighbors of node
	for (tie(i, end) = adjacent_vertices(node, *this->g); i != end; i++)
		if (this->isPartitionColored(*i))
			colors[this->partition[this->getPartition(*i)]] = true;

	// find the first unused color
	for (int i = 0; i < this->numParts; i++) {
		if (!colors[i]) {
			delete[] colors;
			return i;
		}
	}

	// This cant happen, but maybe it will...
	return -1;
}

// Set the prerequisites for ubigraph
#ifdef ubigraph
void Solution::prepareUbigraph() {
	ubigraph_clear();
	ubigraph_set_vertex_style_attribute(0, "shape", "sphere");
	ubigraph_set_edge_style_attribute(0, "strength", "0");
	ubigraph_set_edge_style_attribute(0, "spline", "true");
	ubigraph_set_edge_style_attribute(0, "oriented", "true");
	ubigraph_set_edge_style_attribute(0, "width", "2.0");
	ubigraph_set_edge_style_attribute(0, "color", "#ffffff");

	if (this->numParts < 9) {
		stack<string> shapes = list_of("cone")("cube")("dodecahedron")("icosahedron")("octahedron")("sphere")("tetrahedron")("torus").to_adapter();
		while (!shapes.empty()) {
			ubigraph_new_vertex_style_w_id(shapes.size(), 0);
			ubigraph_set_vertex_style_attribute(shapes.size(), "shape", shapes.top().c_str());
			shapes.pop();
		}
	}
}
#endif

// Read a solution from an input stream in pcp format
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
	
	#ifdef ubigraph
	s->prepareUbigraph();
	#endif

	/// Read partition info and store it into the property map, do the same 
	/// for the "original" vertexID, so they can be compared on all graph
	int i, part;
	for (i = 0; i < vertices; i++) {
		cin >> part;
		s->addVertex(part, i);
		
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
		s->addEdge(source, target);
	}
	
	if (DEBUG_LEVEL > 3)
		cout << "Parsing input finished successfully!" << endl;

	return s;
}

// Read a solution from an input col stream
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
	
	s->numParts = vertices;
	s->partition = new int[vertices];
	s->representatives = new int[vertices];
	s->colorsUsed = vertices;

	#ifdef ubigraph
	s->prepareUbigraph();
	#endif

	if (DEBUG_LEVEL > 3)
		cout << "Reading " << edges << " edges ..." << endl;
		
	if (in.get() != '\n') {
		cerr << "Malformed p-line detected (not terminated by newline)!" << endl;
		return NULL;
	}

	s->partNodes = new vector<Vertex>[vertices];
	
	int i;
	for (i = 0; i < vertices; i++) {
		s->addVertex(i, i);
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

// Read a solution from an input colB stream
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
	
	s->numParts = vertices;
	
	#ifdef ubigraph
	s->prepareUbigraph();
	#endif
	
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
 		out << "\t\tcolor = \"" << hexColors[partition[counter]] << "\"" << endl;
 		out << "\t\tlabel = " << counter++ << endl;
 		out << "\t}" << endl;
 	}
 	
 	graph_traits<Graph>::edge_iterator i, end;
	for (tie(i, end) = edges(*this->g); i != end; i++)
		out << "\t" << lexical_cast<string>(getOriginalId(source(*i, *this->g))) << " -- " << lexical_cast<string>(getOriginalId(target(*i, *this->g))) << ";" << endl;
 	
 	out << "}" << endl;
}

string Solution::toString() {
	stringstream ss;
	
	ss << "{ \"representatives\": [ " << this->representatives[0];
	
	for (int i = 1; i < this->numParts; i++)
		ss << ", " << this->representatives[i];
	
	ss << " ], \"colors\": [ " << this->partition[0];
	for (int i = 1; i < this->numParts; i++)
		ss << ", " << this->partition[i];
	
	ss << " ] }";
	return ss.str();
}

boost::tuple<int, int> Solution::getColorDegreeAndMinColor(Vertex v) {
	vector<bool> colors = vector<bool>(this->numParts, false);
	boost::tuple<int, int> result;
	
	AdjIter i, end;
	for (tie(i, end) = adjacent_vertices(v, *this->g); i != end; i++)
		if (this->isPartitionColored(*i))
			colors[this->partition[this->getPartition(*i)]] = true;

	result.get<0>() = 0;
	result.get<1>() = -1;
	for (vector<bool>::iterator i = colors.begin(); i < colors.end(); i++)
		if (*i)
			result.get<0>()++;
		else if (result.get<1>() < 0)
			result.get<1>() = distance(i, colors.begin());
	
	return result;
}
