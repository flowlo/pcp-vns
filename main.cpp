#include "header/main.hpp"

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

int DEBUG_LEVEL = 2;

int main(int num, char* args[]) {
	ostream* out = &cout;
	istream* in = &cin;
	char* units = "nc";
	
	int unsuccessfulShake = 10, shakeStart = 0, shakeIncrement = 10, maxTime = 10;
	
	/// Argument parsing
	for (int i = 1; i < num; i++) {
		/// Print to specify graphviz output file
		if  (strcmp(args[i], "-n") == 0) {
			i++;
			if (i == num) {
				cerr<<"No VNS unit sequence specified for option -n"<<endl;
				cerr<<"Usage: -n [STRING]"<<endl;
				return -1;
			}
			
			units = args[i];
		}
		else if (strcmp(args[i], "-p") == 0) {
			i++;
			if (i == num) {
				cerr<<"Bad or no filename for option -p"<<endl;
				cerr<<"Usage: -p [FILENAME]"<<endl;
				return -1;
			}
			
			out = new fstream(args[i], fstream::out);
			if (out->fail()) {
				cerr<<"Error when trying to access file: "<<args[i]<<endl;
				return -1;
			}
		}
		else if (strcmp(args[i], "-i") == 0) {
			i++;
			if (i == num) {
				cerr<<"Bad or no filename for option -i"<<endl;
				cerr<<"Usage: -i [FILENAME]"<<endl;
				return -1;
			}
			
			in = new fstream(args[i], fstream::in);
			if (in->fail()) {
				cerr<<"Error when trying to access file: "<<args[i]<<endl;
				return -1;
			}
		}
		/// Specify debug level
		else if (strcmp(args[i], "-d") == 0) {
			i++;
			if (i == num) {
				cerr<<"Bad or no debug level for option -d"<<endl;
				cerr<<"Usage: -d [0-4]"<<endl;
				return -1;
			}
			DEBUG_LEVEL = atoi(args[i]);
			if (DEBUG_LEVEL > 4) {
				DEBUG_LEVEL = 4;
				
				cout << "The specified debug level (-d) exceeds"
				<< " the maximum. It was set to " << DEBUG_LEVEL
				<< "." << endl;
			}
			else if (DEBUG_LEVEL < 0) {
				DEBUG_LEVEL = 0;
			}
		}
		else if (strcmp(args[i], "-u")) {
			i++;
			if (i == num) {
				cerr<<"Bad or no unsuccessful shake threshold for option -u"<<endl;
				cerr<<"Usage: -u [POSITIVE INTEGER]"<<endl;
				return -1;
			}
			unsuccessfulShake = atoi(args[i]);
			
			if (unsuccessfulShake < 0) {
				cerr << "Invalid unsuccessful shake threshold." << endl;
				return -1;
			}
		}
		else if (strcmp(args[i], "-s")) {
			i++;
			if (i == num) {
				cerr<<"Bad or no shake start for option -s"<<endl;
				cerr<<"Usage: -s [POSITIVE INTEGER]"<<endl;
				return -1;
			}
			shakeStart = atoi(args[i]);
			
			if (shakeStart < 0) {
				cerr << "Invalid shake start." << endl;
				return -1;
			}
		}
		else if (strcmp(args[i], "-t")) {
			i++;
			if (i == num) {
				cerr<<"Bad or no time limit for option -t"<<endl;
				cerr<<"Usage: -t [POSITIVE INTEGER]"<<endl;
				return -1;
			}
			maxTime = atoi(args[i]);
			
			if (maxTime < 0) {
				cerr << "Invalid maximum time." << endl;
				return -1;
			}
		}
	}
	
	Solution *fullG = new Solution();
	
	if (!readGraph(*in, *fullG)) {
		cerr << "Error reading from stdin" << endl;
	}
	
	Solution *onestep = onestepCD(*fullG);
	
	if (DEBUG_LEVEL > 2)
		cout<<"Onestep solution uses "<<onestep->colorsUsed<<" colors"<<endl;
	
	Solution *best = vnsRun(*onestep, *fullG, units, unsuccessfulShake, 
								  shakeStart, shakeIncrement, maxTime);
	
	if (best == NULL) {
		return -1;
	}
	
	VertexID_Map vertex_id = get(vertex_index2_t(), *best->g);
	write_graphviz(*out, *best->g, make_label_writer(vertex_id));
	
	if (DEBUG_LEVEL > 1)
		cout<<"Writing complete"<<endl;
	
	out->flush();
	
	if (out != &cout) {
		((fstream*)out)->close();
	}
	if (in != &cin) {
		((fstream*)in)->close();
	}
	return 0;
}
