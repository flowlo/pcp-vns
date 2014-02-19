#include "../include/IO.hpp"

using namespace std;

namespace pcp {
	Solution readSolution(istream& in) {
		// Decide which format the input is in, and parse accordingly
		if (isdigit(in.peek())) {
			string buffer = "";
			getline(in, buffer);
			in.seekg(0);

			if (buffer.find(' ') == string::npos) {
				if (DEBUG_LEVEL > 2)
					cout << "Detected .col.b input." << endl;
				return readColBStream(in);
			}
			else {
				if (DEBUG_LEVEL > 2)
					cout << "Detected .pcp input." << endl;
				return readPcpStream(in);
			}
		}
		else {
			if (DEBUG_LEVEL > 2)
				cout << "Detected .col input." << endl;
			return readColStream(in);
		}
	}

	Solution readPcpStream(istream& in) {
		uint32_t vertices, edges, partition, i;
		VertexIter v, vend;

		//BOOST_LOG_TRIVIAL(info) << "Started parsing PCP-file";	

		in >> vertices >> edges >> partition;
		//BOOST_LOG_TRIVIAL(debug) << "Reading " << vertices << " vertices, " << edges 
		//	<< " edges and " << partition << " partitions";

		Solution ret(vertices, partition);

		Graph& g = ret.getFullGraph();
		for (boost::tie(v, vend) = boost::vertices(g); v != vend; v++) {
			partition_t part;
			in >> part;
			ret.setPartition(*v, part);

			//BOOST_LOG_TRIVIAL(debug) << "Added vertex " << *v << " to partition " << part;
		}

		for (i = 0; i < edges; i++) {
			Vertex source, target;
			in >> source >> target;
			ret.addEdge(source, target);

			//BOOST_LOG_TRIVIAL(debug) << "Added edge (" << source << ", " << target << ")";
		}

		//BOOST_LOG_TRIVIAL(info) << "Finished parsing PCP-file";
		return ret;
	}

	Solution readColStream(istream& in) {
		return Solution();
	}

	Solution readColBStream(istream& in) {
		return Solution();
	}
}
