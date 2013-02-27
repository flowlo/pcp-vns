// This file contains the basic definitions and classes necessary to solve the
// PCP, as well as some useful shortcuts and helper definitions 

#ifndef PCP_H
#define PCP_H
#include <boost/assign/list_of.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/tuple/tuple.hpp>

namespace pcp {
	// Properties that are stored for each vertex in a property map
	// vertex_index1 contains the partitionID for each vertex,
	// vertex_index2 contains the original ID for each vertex
	typedef boost::property<boost::vertex_index2_t, unsigned int, boost::property<
		boost::vertex_index1_t, int> > VertexProperty;

	// Properties that are stored for each edge in the graph
	typedef boost::property<boost::edge_index_t, int> EdgeProperty;

	// Graph is an undirected graph which stores VertexProperty for each
	// vertex and EdgeProperty for each edge
	typedef boost::adjacency_list<boost::vecS, boost::vecS,
		boost::undirectedS, VertexProperty, EdgeProperty> Graph;

	// Represents a vertex
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	
	// Represents an edge
	typedef boost::graph_traits<Graph>::edge_descriptor Edge;

	// Standard vertex_iterator
	typedef boost::graph_traits<Graph>::vertex_iterator VertexIter;

	// Standard vertex_iterator
	typedef boost::graph_traits<Graph>::edge_iterator EdgeIter;
	
	// Iterator for adjacent vertices
	typedef boost::graph_traits<Graph>::adjacency_iterator AdjIter;

	// Property map used to store the "original" ID of a vertex
	typedef boost::property_map<Graph, boost::vertex_index2_t>::type
		VertexID_Map;

	// Property map used to store the partition to which the vertex belongs
	typedef boost::property_map<Graph, boost::vertex_index1_t>::type
		VertexPart_Map;

	const std::vector<std::string> hexColors = boost::assign::list_of("#ff0000")("#00ff00")("#0000ff")("#ffff00")("#00ffff")("#ff00ff")("#ffffff")("#ff8800")("#88ff00")("#0088ff")("#ff0088")("#8800ff")("#00ff88");

	// Stores a Solution for the PCP-Problem
	// Contains the graph representing the solution, the number of
	// partitions, the color used for each partition and the number of
	// colors used for this specific solution.
	class Solution {
		public:
			// Constructs a new empty solution
			Solution();

			// Deep-copies the specified Solution
			Solution(Solution *s);

			// Destructor
			~Solution();

			// Graph representating the PCP instance
			Graph *g;
			
			// Return PartitionID of Vertex v
			int getPartition(Vertex v);
			
			// Return the ID of Vertex v in the original graph
			unsigned int getOriginalId(Vertex v);
			
			// Set the original ID of Vertex v to id i
			void setOriginalId(Vertex v, int id);
			
			// Set the color of the partition to which v belongs to
			void setPartitionColor(Vertex v, int color);
			
			// Get the color of v's partitions
			int getPartitionColor(Vertex v);
			
			// Returns true whether v's partition colored
			bool isPartitionColored(Vertex v);
			
			// Return v's colorDegree, which is specified as the number of adjacent
			// colored nodes
			int getColorDegree(Vertex v);
			
			// Return the minimal feasible color available to node v
			int minPossibleColor(Vertex v);
			
			boost::tuple<int, int> getColorDegreeAndMinColor(Vertex v);
			
			// Print the graph with colors in graphviz format
			void print(std::ostream& out);

			std::string toString();

			// Number of partitions in the PCP instance
			int numParts;

			// Holds the color for each partition
			int *partition;
			
			// Only available in the full, original graph;
			// Contains an array of vectors, representing each node of a 
			// partition
			std::vector<Vertex> *partNodes;

			// Colors used by this solution
			int colorsUsed;
			
			// Provides a matching from partitions to chosen nodes
			int *representatives;
			
			// Keep track of copies of a graph
			int *copyCounter;
			
			// Copy the graph, instead of only the colors
			void requestDeepCopy();
			
			// Construction methods used for building a solution out of specific
			// file types
			static Solution* fromColStream(std::istream& in);
			static Solution* fromColBStream(std::istream& in);
			static Solution* fromPcpStream(std::istream& in);
			
			// Methods for manipulating the graph
			void addVertex(int part, Vertex id);
			void removeVertex(Vertex id);
			void addEdge(Vertex v1, Vertex v2);
			void replaceVertex(Vertex toR, Vertex rep, Solution& full);
			
			#ifdef ubigraph
			void redraw();
			void redraw(int shift);
			void prepareUbigraph();
			#endif
		private:
			boost::property_map<Graph, boost::vertex_index1_t>::type partitionMap;
			boost::property_map<Graph, boost::vertex_index2_t>::type idMap;
	};
}
// Global variable holding the debug level.
// Number between 0 and 4, with 4 being the most verbose.
extern int DEBUG_LEVEL;

#endif
