/// This file contains the implementation of the parser described in parser.hpp

#include "header/parser.hpp"

using namespace boost;
using namespace std;

namespace pcp {
	/// see parser.hpp
	bool readGraph(std::istream& in, Solution& s) {
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
			return false;
		}
		
		/// Initialize the solution to the read parameters
		s.partition = new int[nums[parts]];
		s.representatives = new int[nums[parts]];
		s.numParts = nums[parts];
		s.colorsUsed = nums[parts];

		/// Initialize the property maps for partition and vertexID
		VertexID_Map vertex_id = get(vertex_index2_t(), *s.g);
		VertexPart_Map vertex_part = get(vertex_index1_t(), *s.g);

		
		/// Read partition info and store it into the property map, do the same 
		/// for the "original" vertexID, so they can be compared on all graph
		for (i = 0; i < nums[vertices]; i++) {
			getline(in, buffer);
			Vertex v = add_vertex(*s.g);
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
			add_edge(v1, v2, *s.g);
		}
		if (DEBUG_LEVEL > 3) {
			cout<<"Reading input finished"<<endl;
		}
		return true;
	}	
}
