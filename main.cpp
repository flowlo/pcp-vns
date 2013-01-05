#include "header/main.hpp"

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
	this->partition = new int[this->numParts];
	this->representatives = new int[this->numParts];

	for (int i = 0; i < this->numParts; i++) {
		this->partition[i] = toCopy->partition[i];
		this->representatives[i] = toCopy->representatives[i];
	}
	this->colorsUsed = toCopy->colorsUsed;
	copy_graph(*toCopy->g, *this->g);
}

int DEBUG_LEVEL = 2;

int main(int num, char* args[]) {
	ostream* out = &cout;
	
	/// Argument parsing
	for (int i = 1; i < num; i++) {
		/// Print to specify graphviz output file
		if (strcmp(args[i], "-p") == 0) {
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
		/// Specify debug level
		else if (strcmp(args[i], "-d") == 0) {
			i++;
			if (i == num) {
				cerr<<"Bad or no debug level for option -d"<<endl;
				cerr<<"Usage: -d [0-4]"<<endl;
				return -1;
			}
			DEBUG_LEVEL = atoi(args[i]);
			if (DEBUG_LEVEL > 4)
				DEBUG_LEVEL = 4;
			if (DEBUG_LEVEL < 0)
				DEBUG_LEVEL = 0;
		}
	}
	
	
	Solution *fullG = new Solution();
	
	if (!readGraph(cin, *fullG)) {
		cerr<<"Error reading from stdin"<<endl;
	}
	
	Solution *onestep = onestepCD(*fullG);
	
	
	cout<<"Onestep solution uses "<<onestep->colorsUsed<<" colors"<<endl;
	
	Solution best = vnsRun(*onestep, *fullG, 1, 0, 10, 0);
	VertexID_Map vertex_id = get(vertex_index2_t(), *best.g);
	write_graphviz(*out, *best.g, make_label_writer(vertex_id));
	cout<<"Writing complete"<<endl;
	
	out->flush();
	if (out != &cout) {
		((fstream*)out)->close();
	}
	return 0;
}
