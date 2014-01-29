#include "../include/IO.hpp"

using namespace std;

namespace pcp {
	pcp::Solution fromPcpStream(istream& in) {
		uint32_t vertices, edges, partition, i;
		pcp::VertexIter v, vend;

		//BOOST_LOG_TRIVIAL(info) << "Started parsing PCP-file";	

		in >> vertices >> edges >> partition;
		//BOOST_LOG_TRIVIAL(debug) << "Reading " << vertices << " vertices, " << edges 
		//	<< " edges and " << partition << " partitions";

		pcp::Solution ret(vertices, partition);

		pcp::Graph& g = ret.getFullGraph();
		for (boost::tie(v, vend) = boost::vertices(g); v != vend; v++) {
			partition_t part;
			in >> part;
			ret.setPartition(*v, part);

			//BOOST_LOG_TRIVIAL(debug) << "Added vertex " << *v << " to partition " << part;
		}

		for (i = 0; i < edges; i++) {
			pcp::Vertex source, target;
			in >> source >> target;
			ret.addEdge(source, target);

			//BOOST_LOG_TRIVIAL(debug) << "Added edge (" << source << ", " << target << ")";
		}

		//BOOST_LOG_TRIVIAL(info) << "Finished parsing PCP-file";
		return ret;
	}
}
