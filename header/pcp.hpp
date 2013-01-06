/// This file contains the basic definitions and classes necessary to solve the
/// PCP, as well as some useful shortcuts and helper definitions 

#ifndef PCP_H
#define PCP_H
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>

namespace pcp {
	/// Properties that are stored for each vertex in a property map
	/// vertex_index1 contains the partitionID for each vertex,
	/// vertex_index2 contains the original ID for each vertex
	typedef boost::property<boost::vertex_index2_t, int, boost::property<
		boost::vertex_index1_t, int> > VertexProperty;
									
	/// Properties that are stored for each edge in the graph
	typedef boost::property<boost::edge_index_t, int> EdgeProperty;
	
	/// Graph is an undirected graph which stores VertexProperty for each
	// vertex and EdgeProperty for each edge
	typedef boost::adjacency_list<boost::vecS, boost::vecS,
		boost::undirectedS, VertexProperty, EdgeProperty> Graph;
											
	/// Represents a vertex
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	
	/// Standard vertex_iterator
	typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;
	
	/// Property map used to store the "original" ID of a vertex
	typedef boost::property_map<Graph, boost::vertex_index2_t>::type
		VertexID_Map;
	
	/// Property map used to store the partition to which the vertex belongs
	typedef boost::property_map<Graph, boost::vertex_index1_t>::type
		VertexPart_Map;
	
	/// Stores a Solution for the PCP-Problem
	/// Contains the graph representing the solution, the number of
	/// partitions, the color used for each partition and the number of
	/// colors used for this specific solution.
	class Solution {
		public:
			/// Constructs a new empty solution
			Solution();

			/// Deep-copies the specified Solution
			Solution(Solution *s);

			/// Graph representating the PCP instance
			Graph *g;

			/// Number of partitions in the PCP instance
			int numParts;

			/// Holds the color for each partition
			int *partition;

			/// Colors used by this solution
			int colorsUsed;
			
			/// Provides a matching from partitions to chosen nodes
			int *representatives;
	};
}
/// Global variable holding the debug level.
/// Number between 0 and 4, with 4 being the most verbose.
extern int DEBUG_LEVEL;

#endif